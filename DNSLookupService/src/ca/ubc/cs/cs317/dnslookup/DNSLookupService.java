package ca.ubc.cs.cs317.dnslookup;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.*;
import java.util.*;

public class DNSLookupService {

    public static final int DEFAULT_DNS_PORT = 53;
    private static final int MAX_INDIRECTION_LEVEL_NS = 10;
    private static final int MAX_QUERY_ATTEMPTS = 3;
    private static final int MAX_DNS_MESSAGE_LENGTH = 512;
    private static final int SO_TIMEOUT = 5000;

    private final DNSCache cache = DNSCache.getInstance();
    private final Random random = new Random();
    private final DNSVerbosePrinter verbose;
    private final DatagramSocket socket;

    private int indirectionLevel = MAX_INDIRECTION_LEVEL_NS;
    /**
     * Creates a new lookup service. Also initializes the datagram socket object with a default timeout.
     *
     * @param verbose    A DNSVerbosePrinter listener object with methods to be called at key events in the query
     *                   processing.
     * @throws SocketException      If a DatagramSocket cannot be created.
     * @throws UnknownHostException If the nameserver is not a valid server.
     */
    public DNSLookupService(DNSVerbosePrinter verbose) throws SocketException, UnknownHostException {
        this.verbose = verbose;
        socket = new DatagramSocket();
        socket.setSoTimeout(SO_TIMEOUT);
    }

    /**
     * Closes the lookup service and related sockets and resources.
     */
    public void close() {
        socket.close();
    }

    /**
     * Examines a set of resource records to see if any of them are an answer to the given question.
     *
     * @param rrs       The set of resource records to be examined
     * @param question  The DNS question
     * @return          true if the collection of resource records contains an answer to the given question.
     */
    private boolean containsAnswer(Collection<ResourceRecord> rrs, DNSQuestion question) {
        for (ResourceRecord rr : rrs) {
            if (rr.getQuestion().equals(question) && rr.getRecordType() == question.getRecordType()) {
                return true;
            }
        }
        return false;
    }

    /**
     * Finds all the results for a specific question. If there are valid (not expired) results in the cache, uses these
     * results, otherwise queries the nameserver for new records. If there are CNAME records associated to the question,
     * they are retrieved recursively for new records of the same type, and the returning set will contain both the
     * CNAME record and the resulting resource records of the indicated type.
     *
     * @param question             Host and record type to be used for search.
     * @param maxIndirectionLevels Number of CNAME indirection levels to support.
     * @return A set of resource records corresponding to the specific query requested.
     * @throws DNSErrorException If the number CNAME redirection levels exceeds the value set in
     *                           maxIndirectionLevels.
     */
    public Collection<ResourceRecord> getResultsFollowingCNames(DNSQuestion question, int maxIndirectionLevels)
            throws DNSErrorException {

        if (maxIndirectionLevels < 0) throw new DNSErrorException("CNAME indirection limit exceeded");

        Collection<ResourceRecord> directResults = iterativeQuery(question);
        if (containsAnswer(directResults, question)) {
            return directResults;
        }

        Set<ResourceRecord> newResults = new HashSet<>();
        for (ResourceRecord record : directResults) {
            newResults.add(record);
            if (record.getRecordType() == RecordType.CNAME) {
                newResults.addAll(getResultsFollowingCNames(
                        new DNSQuestion(record.getTextResult(), question.getRecordType(), question.getRecordClass()),
                        maxIndirectionLevels - 1));
            }
        }
        return newResults;
    }

    /**
     * Answers one question.  If there are valid (not expired) results in the cache, returns these results.
     * Otherwise it chooses the best nameserver to query, retrieves results from that server
     * (using individualQueryProcess which adds all the results to the cache) and repeats until either:
     *   the cache contains an answer to the query, or
     *   the cache contains an answer to the query that is a CNAME record rather than the requested type, or
     *   every "best" nameserver in the cache has already been tried.
     *
     *  @param question Host name and record type/class to be used for the query.
     */
    public Collection<ResourceRecord> iterativeQuery(DNSQuestion question)
            throws DNSErrorException {

        List<ResourceRecord> cachedResults = cache.getCachedResults(question);

        //Get all name servers for question (will most likely be root servers)
        List<ResourceRecord> servers = cache.getBestNameservers(question);

        //Set first server in fifo order, also set initial inet address
        ResourceRecord server = servers.get(0);
        InetAddress address = server.getInetResult();

        //loop while cachedresults for our question is empty
        while(cachedResults.isEmpty()) {

            //get known ips for our name server
            List<ResourceRecord> ips = cache.filterByKnownIPAddress(Arrays.asList(server));

            //If no ips are known, look for more info on this name server
            if(ips.isEmpty()) {
                iterativeQuery(new DNSQuestion(server.getTextResult(),question.getRecordType(), question.getRecordClass()));
            } else {
                // else, set address to known ip and query it with our question
                address = ips.get(0).getInetResult();
                individualQueryProcess(question, address);
            }

            //set the next best name servers from cache and set the next server
            servers = cache.getBestNameservers(question);
            server = servers.get(0);

            //refresh results to check if we found our answer
            cachedResults = cache.getCachedResults(question);
        }

        return new HashSet<>(cachedResults);
    }


    /**
     * Handles the process of sending an individual DNS query with a single question. Builds and sends the query (request)
     * message, then receives and parses the response. Received responses that do not match the requested transaction ID
     * are ignored. If no response is received after SO_TIMEOUT milliseconds, the request is sent again, with the same
     * transaction ID. The query should be sent at most MAX_QUERY_ATTEMPTS times, after which the function should return
     * without changing any values. If a response is received, all of its records are added to the cache.
     * <p>
     * If the reply contains a non-zero Rcode value, then throw a DNSErrorException.
     * <p>
     * The method verbose.printQueryToSend() must be called every time a new query message is about to be sent.
     *
     * @param question Host name and record type/class to be used for the query.
     * @param server   Address of the server to be used for the query.
     * @return If no response is received, returns null. Otherwise, returns a set of all resource records
     * received in the response.
     * @throws DNSErrorException if the Rcode in the response is non-zero
     */
    public Set<ResourceRecord> individualQueryProcess(DNSQuestion question, InetAddress server)
            throws DNSErrorException {

        //Build output msg from question
        DNSMessage msg = buildQuery(question);

        //Try Receiving response MAX_QUERY_ATTEMPTS times
        for(int i = 0; i < MAX_QUERY_ATTEMPTS; i++) {
            try {

                //Print to verbose
                verbose.printQueryToSend("UDP", question, server, msg.getID());

                //Create output msg packet and send to socket
                DatagramPacket packet = new DatagramPacket(msg.getUsed(), msg.getUsed().length, server, DEFAULT_DNS_PORT);
                socket.send(packet);


                //wait for responses, if response with wrong id returned or not response, keep waiting
                DNSMessage res;
                do {
                    //Create response packet buffer
                    byte[] recvBuf = new byte[MAX_DNS_MESSAGE_LENGTH];
                    DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);

                    //Wait for response, once received save data as DNSMessage
                    socket.receive(recvPacket);
                    res = new DNSMessage(recvPacket.getData(), recvPacket.getLength());
                } while(res.getID() != msg.getID() || !res.getQR());

                //check for TC flag
                if(res.getTC()) {
                    //since we are exiting normal response, Check for rCode and print initial response header before we try TCP
                    if(res.getRcode() != 0) throw new DNSErrorException(DNSMessage.dnsErrorMessage(res.getRcode()));
                    verbose.printResponseHeaderInfo(res.getID(),res.getAA(), res.getTC(), res.getRcode());

                    return individualQueryProcessTCP(question, server);
                }

                //process response and return
                return(processResponse(res));
            } catch(SocketTimeoutException e) {
                //Datagram Socket timeout
            } catch(DNSErrorException e) {
                throw new DNSErrorException(e.getMessage());
            } catch(Exception e) {
                //lmao
                //System.out.println(e.toString());
            }
        }
        return null;
    }

    /**
     * Creates a DNSMessage containing a DNS query.
     * A random transaction ID must be generated and filled in the corresponding part of the query. The query
     * must be built as an iterative (non-recursive) request for a regular query with a single question. When the
     * function returns, the message's buffer's position (`message.buffer.position`) must be equivalent
     * to the size of the query data.
     *
     * @param question    Host name and record type/class to be used for the query.
     * @return The DNSMessage containing the query.
     */
    public DNSMessage buildQuery(DNSQuestion question) {
        DNSMessage message = new DNSMessage((short)random.nextInt(32767));
        message.addQuestion(question);
        return message;
    }

    /**
     * Parses and processes a response received by a nameserver.
     * If the reply contains a non-zero Rcode value, then throw a DNSErrorException.
     * Adds all resource records found in the response message to the cache.
     * Calls methods in the verbose object at appropriate points of the processing sequence. Must be able
     * to properly parse records of the types: A, AAAA, NS, CNAME and MX (the priority field for MX may be ignored). Any
     * other unsupported record type must create a record object with the data represented as a hex string (see method
     * byteArrayToHexString).
     *
     * @param message The DNSMessage received from the server.
     * @return A set of all resource records received in the response.
     * @throws DNSErrorException if the Rcode value in the reply header is non-zero
     */
    public Set<ResourceRecord> processResponse(DNSMessage message) throws DNSErrorException {
        //Throw error if rCode is not 0
        if(message.getRcode() != 0) throw new DNSErrorException(DNSMessage.dnsErrorMessage(message.getRcode()));

        Set<ResourceRecord> res = new HashSet<>();

        //Print Response Header and move buf pointer past questions section
        verbose.printResponseHeaderInfo(message.getID(),message.getAA(), message.getTC(), message.getRcode());
        for (int i = 0; i < message.getQDCount(); i++) {
            message.getQuestion();
        }

        //Print answers header and answers, also cache answer results
        verbose.printAnswersHeader(message.getANCount());
        for (int i = 0; i < message.getANCount(); i++) {
            ResourceRecord rec = message.getRR();
            res.add(rec);
            cache.addResult(rec);
            verbose.printIndividualResourceRecord(rec,rec.getRecordType().getCode(),rec.getRecordClass().getCode());
        }

        //Print Name servers header and name serves, also cache name servers
        verbose.printNameserversHeader(message.getNSCount());
        for (int i = 0; i < message.getNSCount(); i++) {
            ResourceRecord rec = message.getRR();
            res.add(rec);
            cache.addResult(rec);
            verbose.printIndividualResourceRecord(rec,rec.getRecordType().getCode(),rec.getRecordClass().getCode());
        }

        //Print additional info header and additional info, also cache additional info
        verbose.printAdditionalInfoHeader(message.getARCount());
        for (int i = 0; i < message.getARCount(); i++) {
            ResourceRecord rec = message.getRR();
            res.add(rec);
            cache.addResult(rec);
            verbose.printIndividualResourceRecord(rec,rec.getRecordType().getCode(),rec.getRecordClass().getCode());
        }

        return res;
    }
    private Set<ResourceRecord> individualQueryProcessTCP(DNSQuestion question, InetAddress server)
            throws DNSErrorException {

        //Build output msg from question
        DNSMessage msg = buildQuery(question);

        //Try Receiving response MAX_QUERY_ATTEMPTS times
        try {

            //Print to verbose
            verbose.printQueryToSend("TCP", question, server, msg.getID());

            //Create socket and init streams
            Socket socket = new Socket(server, DEFAULT_DNS_PORT);
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            DataInputStream in = new DataInputStream(socket.getInputStream());

            //Write length to stream then write data
            out.writeShort(msg.getUsed().length);
            out.write(msg.getUsed());
            out.flush();

            //read length then read data
            int len = in.readShort();
            byte[] buf = new byte[len];
            in.readFully(buf,0,len);

            //create DNS message
            DNSMessage res = new DNSMessage(buf, len);

            //check for inconsistencies
            if(res.getID() != msg.getID() || !res.getQR()){
                throw new DNSErrorException("Error getting response with TCP");
            }

            //close socket
            socket.close();

            //process response and return
            return(processResponse(res));
        } catch(SocketTimeoutException e) {
            //Datagram Socket timeout
        } catch(DNSErrorException e) {
            throw new DNSErrorException(e.getMessage());
        } catch(Exception e) {
            //lmao
            //System.out.println(e.toString());
        }

        return null;
    }

    public static class DNSErrorException extends Exception {
        public DNSErrorException(String msg) {
            super(msg);
        }
    }
}


