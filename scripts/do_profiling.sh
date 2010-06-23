#!/bin/bash

#############################################################################
# This script generates several files containing the result of profiling    #
# blockem against a set of examples contained in src/tests/examples/games   #
# It saves the results in blockem-path/src/tests/gprof. One file per        #
# blockem example game file (some of them take less time to process than    #
# others)                                                                   #
# Profiling allows you to learn where your program spent its time and which #
# functions called which other functions while it was executing             #
#############################################################################

if [ $# -eq 1 ]; then
    if [ $1 = "--help" ]; then
        echo "This script generates several files containing the result of profiling"
        echo "blockem against a set of examples contained in src/tests/examples/games"
        echo "It saves the results in blockem-path/src/tests/gprof. One file per"
        echo "blockem example game file (some of them take less time to process than"
        echo "others)"
        echo "Profiling allows you to learn where your program spent its time and which"
        echo "functions called which other functions while it was executing"
    else
        echo "bad parameter. Try '--help'"
    fi
    
    exit 0
fi

GAME_INPUT_DIR=../examples/games
DEPTH=3

#cd into base directory
cd ../

if [ ! -f src/blockem ]; then
	echo "You must compile blockem before trying to profile it man..."
	exit
fi

#cd now into profiling base directory
cd src/tests/gprof
# copy binary file to here
cp -f ../../blockem .
chmod u+x ./blockem

for THIS_FILE in `find $GAME_INPUT_DIR -maxdepth 1 -type f -not -iname "*error*"`; do
    BASENAME=`basename $THIS_FILE`
    echo "processing ./blockem --mode=2 -d $DEPTH $THIS_FILE"
    rm -f gmon.out
    ./blockem --mode=2 -d $DEPTH $THIS_FILE

    date > $BASENAME.gprof
    echo "./blockem --mode=2 -d $DEPTH $THIS_FILE" >> $BASENAME.gprof
    echo "" >> $BASENAME.gprof
    if [ -f gmon.out ]; then
        gprof ./blockem gmon.out >> $BASENAME.gprof
    else
        echo "gmon.out couldn't be found. Are you sure you compiled with --enable-gprof?"
    fi
done

rm -f ./gmon.out
rm -f ./blockem
