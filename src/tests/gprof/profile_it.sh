#!/bin/bash

GAME_INPUT_DIR=../examples/games
DEPTH=3

if [ ! -f ../../blockem ] 
then
	echo "You must compile blockem before packaging it man..."
	exit
fi

cp -f ../../blockem .
chmod u+x ./blockem

for THIS_FILE in `ls $GAME_INPUT_DIR`; do
    echo "processing ./blockem -d $DEPTH -c $GAME_INPUT_DIR/$THIS_FILE"
    rm -f gmon.out
    ./blockem -d $DEPTH -c $GAME_INPUT_DIR/$THIS_FILE

    date > $THIS_FILE.gprof
    echo "./blockem -d $DEPTH -c $GAME_INPUT_DIR/$THIS_FILE" >> $THIS_FILE.gprof
    echo "" >> $THIS_FILE.gprof
    gprof ./blockem gmon.out >> $THIS_FILE.gprof    
done

rm -f ./gmon.out
rm -f ./blockem
