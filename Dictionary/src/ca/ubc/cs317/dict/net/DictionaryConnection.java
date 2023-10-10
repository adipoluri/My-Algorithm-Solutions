package ca.ubc.cs317.dict.net;

import ca.ubc.cs317.dict.model.Database;
import ca.ubc.cs317.dict.model.Definition;
import ca.ubc.cs317.dict.model.MatchingStrategy;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.*;

import static ca.ubc.cs317.dict.net.DictStringParser.splitAtoms;

/**
 * Created by Jonatan on 2017-09-09.
 */
public class DictionaryConnection {

    private static final int DEFAULT_PORT = 2628;

    private Socket socket;
    private BufferedReader in;
    private PrintWriter out;

    private Boolean verbose = false; //Enables full console printing

    /** Establishes a new connection with a DICT server using an explicit host and port number, and handles initial
     * welcome messages.
     *
     * @param host Name of the host where the DICT server is running
     * @param port Port number used by the DICT server
     * @throws DictConnectionException If the host does not exist, the connection can't be established, or the messages
     * don't match their expected value.
     */
    public DictionaryConnection(String host, int port) throws DictConnectionException {
        try {
            //Create Socket
            socket = new Socket(host, port);

            //create input and output
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            //check initial connection response code
            Status res = Status.readStatus(in);
            if (res.getStatusCode() != 220) {
                throw new DictConnectionException("Error Code " + res.getStatusCode());
            }

            System.out.println("Successfully Connected");
        } catch(Exception e) {
            throw new DictConnectionException("Error Occurred");
        }
    }

    /** Establishes a new connection with a DICT server using an explicit host, with the default DICT port number, and
     * handles initial welcome messages.
     *
     * @param host Name of the host where the DICT server is running
     * @throws DictConnectionException If the host does not exist, the connection can't be established, or the messages
     * don't match their expected value.
     */
    public DictionaryConnection(String host) throws DictConnectionException {
        this(host, DEFAULT_PORT);
    }

    /** Sends the final QUIT message and closes the connection with the server. This function ignores any exception that
     * may happen while sending the message, receiving its reply, or closing the connection.
     *
     */
    public synchronized void close() {
        try {
            //Send QUIT command
            out.println("QUIT");

            //Send console msg if close response success received AND clear out buffer??
            Status res = Status.readStatus(in);
            if (res.getStatusCode()  != 221) {
                res = Status.readStatus(in);
            }
            System.out.println("Successfully Closed");

            socket.close();
        } catch(Exception e) {
            //Ignore all exceptions
        }
    }

    /** Requests and retrieves all definitions for a specific word.
     *
     * @param word The word whose definition is to be retrieved.
     * @param database The database to be used to retrieve the definition. A special database may be specified,
     *                 indicating either that all regular databases should be used (database name '*'), or that only
     *                 definitions in the first database that has a definition for the word should be used
     *                 (database '!').
     * @return A collection of Definition objects containing all definitions returned by the server.
     * @throws DictConnectionException If the connection was interrupted or the messages don't match their expected value.
     */
    public synchronized Collection<Definition> getDefinitions(String word, Database database) throws DictConnectionException {
        Collection<Definition> set = new ArrayList<>();
        try{
            //Send DEFINE command with "" to support multiple word words
            out.println("DEFINE " + database.getName() + " \"" + word + "\"");
            Status res = Status.readStatus(in);
            switch(res.getStatusCode()){
                case 150:
                    System.out.println(res.getDetails());

                    //Iterate through definition Responses (Code 151) until Code 250 is reached
                    Status resQuery = Status.readStatus(in);
                    while (resQuery.getStatusCode() == 151) {

                        //Create definition object with Database and word
                        String[] data = splitAtoms(resQuery.getDetails());
                        Definition definition = new Definition(data[0],data[1]);

                        //Iterate through text by line and add match until terminal . is reached
                        String input;
                        while(!Objects.equals(input = in.readLine(), ".")) {
                            //Append definition into definition object
                            definition.appendDefinition(input);
                        }

                        //Add Definition to set
                        set.add(definition);

                        //Read Status of Next definition or Code 250
                        resQuery = Status.readStatus(in);
                    }

                    //Check for 250 response at the end of list
                    if (resQuery.getStatusCode() != 250) {
                        throw new DictConnectionException("250 Not Received");
                    }

                    break;
                case 550:
                    System.out.println("Invalid Database");
                    break;
                case 552:
                    System.out.println("No Matches");
                    break;
                default:
                    throw new DictConnectionException("Invalid response code received");
            }
        } catch (Exception e) {
            throw new DictConnectionException("Error getting Definitions");
        }

        return set;
    }

    /** Requests and retrieves a list of matches for a specific word pattern.
     *
     * @param word     The word whose definition is to be retrieved.
     * @param strategy The strategy to be used to retrieve the list of matches (e.g., prefix, exact).
     * @param database The database to be used to retrieve the definition. A special database may be specified,
     *                 indicating either that all regular databases should be used (database name '*'), or that only
     *                 matches in the first database that has a match for the word should be used (database '!').
     * @return A set of word matches returned by the server.
     * @throws DictConnectionException If the connection was interrupted or the messages don't match their expected value.
     */
    public synchronized Set<String> getMatchList(String word, MatchingStrategy strategy, Database database) throws DictConnectionException {
        Set<String> set = new LinkedHashSet<>();

        try{
            //Send MATCH command with "" to support multiple word words
            out.println("MATCH " + database.getName() + " " +  strategy.getName() + " \"" + word + "\"");
            Status res = Status.readStatus(in);
            switch(res.getStatusCode()){
                case 152:
                    System.out.println(res.getDetails());

                    //Iterate through text by line and add match until terminal . is reached
                    String input;
                    while(!Objects.equals(input = in.readLine(), ".")) {
                        if(verbose) System.out.println("Adding Match: " + input);

                        //Split text data into array
                        String[] data = splitAtoms(input);

                        //insert words into set
                        set.add(data[1]);
                    }

                    //Check for 250 response at the end of list
                    Status resOK = Status.readStatus(in);
                    if (resOK.getStatusCode() != 250) {
                        throw new DictConnectionException("250 Not Received");
                    }
                    break;
                case 550:
                    System.out.println("Invalid Database");
                    break;
                case 551:
                    System.out.println("Invalid Strategy");
                    break;
                case 552:
                    System.out.println("No Matches");
                    break;
                default:
                    throw new DictConnectionException("Invalid response code received");
            }
        } catch (Exception e) {
            throw new DictConnectionException("Error getting Matches");
        }
        return set;
    }

    /** Requests and retrieves a map of database name to an equivalent database object for all valid databases used in the server.
     *
     * @return A map of Database objects supported by the server.
     * @throws DictConnectionException If the connection was interrupted or the messages don't match their expected value.
     */
    public synchronized Map<String, Database> getDatabaseList() throws DictConnectionException {
        Map<String, Database> databaseMap = new HashMap<>();

        try{
            out.println("SHOW DB");
            Status res = Status.readStatus(in);
            switch(res.getStatusCode()){
                case 110:
                    System.out.println(res.getDetails());

                    //Iterate through text by line and add database until terminal . is reached
                    String input;
                    while(!Objects.equals(input = in.readLine(), ".")) {
                        if(verbose) System.out.println("Adding Database: " + input);

                        //Split text data into array
                        String[] data = splitAtoms(input);

                        //insert Database into map
                        databaseMap.put(data[0], new Database(data[0],data[1]));
                    }

                    //Check for 250 response at the end of list
                    Status resOK = Status.readStatus(in);
                    if (resOK.getStatusCode() != 250) {
                        throw new DictConnectionException("250 Not Received");
                    }
                    break;
                case 554:
                    System.out.println("No Databases Present");
                    break;
                default:
                    throw new DictConnectionException("Invalid response code received");
            }
        } catch (Exception e) {
            throw new DictConnectionException("Error getting Databases");
        }

        return databaseMap;
    }

    /** Requests and retrieves a list of all valid matching strategies supported by the server.
     *
     * @return A set of MatchingStrategy objects supported by the server.
     * @throws DictConnectionException If the connection was interrupted or the messages don't match their expected value.
     */
    public synchronized Set<MatchingStrategy> getStrategyList() throws DictConnectionException {
        Set<MatchingStrategy> set = new LinkedHashSet<>();

        try{
            out.println("SHOW STRAT");
            Status res = Status.readStatus(in);
            switch(res.getStatusCode()){
                case 111:
                    System.out.println(res.getDetails());

                    //Iterate through text by line and add strategy until terminal . is reached
                    String input;
                    while(!Objects.equals(input = in.readLine(), ".")) {
                        if(verbose) System.out.println("Adding Strategy: " + input);

                        //Split text data into array
                        String[] data = splitAtoms(input);

                        //insert Strategy into set
                        set.add(new MatchingStrategy(data[0],data[1]));
                    }

                    //Check for 250 response at the end of list
                    Status resOK = Status.readStatus(in);
                    if (resOK.getStatusCode() != 250) {
                        throw new DictConnectionException("250 Not Received");
                    }
                    break;
                case 555:
                    System.out.println("No Strategies Available");
                    break;
                default:
                    throw new DictConnectionException("Invalid response code received");
            }
        } catch (Exception e) {
            throw new DictConnectionException("Error getting Strategies");
        }

        return set;
    }

    /** Requests and retrieves detailed information about the currently selected database.
     *
     * @return A string containing the information returned by the server in response to a "SHOW INFO <db>" command.
     * @throws DictConnectionException If the connection was interrupted or the messages don't match their expected value.
     */
    public synchronized String getDatabaseInfo(Database d) throws DictConnectionException {
	StringBuilder sb = new StringBuilder();

        try{
            out.println("SHOW INFO " + d.getName());
            Status res = Status.readStatus(in);
            switch(res.getStatusCode()){
                case 112:
                    System.out.println(res.getDetails());

                    //Iterate through text by line and add until terminal . is reached
                    String input;
                    while(!Objects.equals(input = in.readLine(), ".")) {
                        //insert text line into string builder
                        sb.append(input+"\n");
                    }

                    //Check for 250 response at the end of list
                    Status resOK = Status.readStatus(in);
                    if (resOK.getStatusCode() != 250) {
                        throw new DictConnectionException("250 Not Received");
                    }
                    break;
                case 550:
                    System.out.println("Invalid Database");
                    break;
                default:
                    throw new DictConnectionException("Invalid response code received");
            }
        } catch (Exception e) {
            throw new DictConnectionException("Error getting database info");
        }

        return sb.toString();
    }
}
