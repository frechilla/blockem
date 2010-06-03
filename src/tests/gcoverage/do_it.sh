#!/bin/bash

OUTPUT_DIR=gcov`date +%Y%m%d%H%M%S`
GCOV_OUTPUT_NAME=blockem.gcov
LCOV_OUTPUT_NAME=blockem.lcov
#GAME_INPUT_DIR=../examples/games
#DEPTH=3

if [ ! -f ../../blockem ] 
then
	echo "You must compile blockem before launching the code coverage utility man..."
	exit
fi
if [ ! -f ../blockemtest ] 
then
	echo "You must compile blockem before launching the code coverage utility man..."
	exit
fi

type lcov &> /dev/null ||  { echo "lcov is required but it's not installed.  Aborting." >&2; exit 1; }

# reset lcov counters
lcov --directory ../ --zerocounters
lcov --directory ../.. --zerocounters

# apps must be run from their directories or counters won't be updated properly
cd ../
./blockemtest

# run gcov through all the files generated when running the apps
mkdir -p ./gcoverage/$OUTPUT_DIR
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov >> gcoverage/$OUTPUT_DIR/$GCOV_OUTPUT_NAME
cd ../
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov > tests/gcoverage/$OUTPUT_DIR/$GCOV_OUTPUT_NAME

## NOTE this is not needed, though I prefered to let the code here in case it is needed in the future
# move all the files to a tmp dir so lcov can access all of them
#TMPDIR=$OUTPUT_NAME.tmp
#mkdir -p ./tests/gcoverage/$TMPDIR
#cd ./tests/gcoverage/$TMPDIR
#find ../../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;
#find ../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;

# generate the HTML. It needs the following 2 steps
lcov  -d . -c -o tests/gcoverage/$OUTPUT_DIR/$LCOV_OUTPUT_NAME
genhtml -t "Blockem Tests Code Coverage" -o tests/gcoverage/$OUTPUT_DIR/ tests/gcoverage/$OUTPUT_DIR/$LCOV_OUTPUT_NAME

