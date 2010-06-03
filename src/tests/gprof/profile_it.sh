#!/bin/bash

GAME_INPUT_DIR=../examples/games
DEPTH=3

if [ ! -f ../../blockem ]; then
	echo "You must compile blockem before trying to profile it man..."
	exit
fi

cp -f ../../blockem .
chmod u+x ./blockem

for THIS_FILE in `find $GAME_INPUT_DIR -maxdepth 1 -type f -not -iname "*error*"`; do
    BASENAME=`basename $THIS_FILE`
    echo "processing ./blockem -d $DEPTH -c $THIS_FILE"
    rm -f gmon.out
    ./blockem -d $DEPTH -c $THIS_FILE

    date > $BASENAME.gprof
    echo "./blockem -d $DEPTH -c $THIS_FILE" >> $BASENAME.gprof
    echo "" >> $BASENAME.gprof
    if [ -f gmon.out ]; then
        gprof ./blockem gmon.out >> $BASENAME.gprof
    else
        echo "gmon.out couldn't be found. Are you sure you compiled with --enable-gprof?"
    fi
done

rm -f ./gmon.out
rm -f ./blockem
