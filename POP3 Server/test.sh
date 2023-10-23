#!/bin/bash

reset() 
{    
    rm -rf mail.store
}

port=$(id | sed -e 's/uid=//' -e 's/(.*$//')
port=$(expr $port % \( 65536 - 1024 \) + 1024)
echo "Using port number $port"
logfile=log.$$.txt
if [ "$1" != "" ] ; then
    pattern=in.p.$1
else
    pattern=in.p.?
fi
for i in $pattern ; do
    echo Running test $i
    inmailstore=$i.mail.store
    expfile=$(echo $i | sed -e 's/in\./exp./')
    outfile=$(echo $i | sed -e 's/in\./out./')
    expmailstore=$expfile.mail.store
    reset
    if [ -d $inmailstore ] ; then
        cp -pr $inmailstore mail.store
    fi
    pkill mypopd
    ./mypopd $port >& $logfile &
    sleep 1
    nc localhost $port -w 3 < $i > $outfile
    sleep 1
    pkill mypopd
    if diff -c $expfile $outfile ; then
        rm -f $outfile
    fi
    rm -f $logfile
    if [ -d $expmailstore ] ; then
        if [ -d mail.store ] ; then
            diff -r -c $expmailstore mail.store
        else
            echo "Test expected mail.store to exist but it does not."
        fi
    fi
done
