#!/bin/bash

#############################################################################
# This script generates several files containing the result of profiling    #
# blockem against a set of examples contained in src/tests/examples/games   #
# It saves the results in the current path in a directory called            #
# gprofYYYYMMDDHHSS. This output directory will contain one file per        #
# blockem example game file (some of them take less time to process than    #
# others)                                                                   #
# Profiling allows you to learn where your program spent its time and which #
# functions called which other functions while it was executing             #
# 
# IMPORTANT: Make sure you compiled blockem with support for gprof. This is #
# done running the configure script with the following parameter:           #
#     './configure --enable-gprof'                                          #
#                                                                           #
# Bear in mind results can be very different if you compile blockem in      #
# debug (that is, with no gcc optimisation) or if you compile it with full  #
# optimisation. Have a look at the --enable-debug option to enable debug    #
#############################################################################

if [ $# -eq 1 ]; then
    if [ $1 = "--help" ]; then
        echo "This script generates several files containing the result of profiling"
        echo "blockem against a set of examples contained in src/tests/examples/games"
        echo "It saves the results in the current path in a directory called"
        echo "gprofYYYYMMDDHHSS. This output directory will contain one file per"
        echo "blockem example game file (some of them take less time to process than"
        echo "others)"
        echo "Profiling allows you to learn where your program spent its time and which"
        echo "functions called which other functions while it was executing"
        echo ""
        echo "IMPORTANT: Make sure you compiled blockem with support for gprof. This is"
        echo "done running the configure script with the following parameter:"
        echo "    './configure --enable-gprof'"
        echo ""
        echo "Bear in mind results can be very different if you compile blockem in"
        echo "debug (that is, with no gcc optimisation) or if you compile it with full"
        echo "optimisation. Have a look at the --enable-debug option to enable debug"
    else
        echo "bad parameter. Try '$0 --help'"
    fi
    
    exit 0
fi

OUTPUT_DIR=gprof`date +%Y%m%d%H%M%S`
GAME_INPUT_DIR=../src/tests/examples/games/
PATH_TO_BIN=../src/blockem
DEPTH=3

if [ ! -f $PATH_TO_BIN ]; then
	echo "You must compile blockem before profiling it. Run '$0 --help' for more help"
	exit
fi

# make output dir
mkdir $OUTPUT_DIR
# copy binary file to here
cp -f $PATH_TO_BIN $OUTPUT_DIR/
# cd into the output directory
cd $OUTPUT_DIR
# binary file must be executable
chmod u+x ./blockem

for THIS_FILE in `find ../$GAME_INPUT_DIR -maxdepth 1 -type f -not -iname "*error*"`; do
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

