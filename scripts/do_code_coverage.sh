#!/bin/bash

#####################################################################
# This script generates html based code coverage report in          #
# blockem-path/src/tests/gcoverage/gcovYYYYMMDDHHSS                 #
# You can browse it opening the index.html file stored in that path #
# This script doesn't take any parameters by command line           #
#####################################################################

if [ $# -eq 1 ]; then
    if [ $1 = "--help" ]; then
        echo "This script generates html based code coverage report in"
        echo "blockem-path/src/tests/gcoverage/gcovYYYYMMDDHHSS"
        echo "You can browse it opening the index.html file stored in that path"
        echo "This script doesn't take any parameters by command line"
    else
        echo "bad parameter. Try '--help'"
    fi
    
    exit 0
fi

OUTPUT_DIR=gcov`date +%Y%m%d%H%M%S`
GCOV_OUTPUT_NAME=blockem.gcov
LCOV_OUTPUT_NAME=blockem.lcov
DEPTH=3

#cd into base directory
cd ../

# test if the binaries exist
if [ ! -f src/blockem ]; then
	echo "You must compile blockem before launching the code coverage utility man..."
	exit
fi
if [ ! -f src/tests/blockemtest ] ; then
	echo "You must compile blockem before launching the code coverage utility man..."
	exit
fi

# lcov is required to generate the fancy looking html
type lcov &> /dev/null ||  { echo "lcov is required but it's not installed.  Aborting." >&2; exit 1; }

# reset lcov counters
lcov --directory src/ --zerocounters
lcov --directory src/tests/ --zerocounters

# apps must be run from their directories or counters won't be updated properly
# firstly run the blockem test application
cd src/tests
echo "running blockem test application in src/tests"
./blockemtest

# run blockem in console mode against the game examples. cd into src/ since blockem
# must be run from there
cd ../
LIST_OF_FILES=`find tests/examples/games -maxdepth 1 -type f -not -iname "*error*"`
echo "running ./blockem -d $DEPTH $LIST_OF_FILES"
./blockem --mode=2 -d $DEPTH $LIST_OF_FILES 

# now test the command line options
# help & version message
./blockem --help
./blockem --version
# parsing command line error
./blockem --force-parsing-error
# bad mode option
./blockem --mode=3
# several errors due to incompatible command line options
./blockem --mode=1 -r 0 -c 0
./blockem --mode=1 -r 0 -c 0 -x 4 -y 4
./blockem --mode=1 -r 14 -c 14 -x 15 -y 4
./blockem --mode=1 -r 14 -c 14 -x 4 -y 14
# computer solves this
./blockem --mode=1 -r 14 -c 14 -x 4 -y 4
# impossible to solve
./blockem --mode=1 -r 1 -c 1 -x 0 -y 0


# run gcov through all the files generated when running the apps
# gcov must be run from the directory where the app is created by the makefile
mkdir -p ./tests/gcoverage/$OUTPUT_DIR
# files in the 'tests' directory
cd tests/
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov >> ./gcoverage/$OUTPUT_DIR/$GCOV_OUTPUT_NAME
# now the src/ dir
cd ../
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov > ./tests/gcoverage/$OUTPUT_DIR/$GCOV_OUTPUT_NAME


## NOTE this is not needed, though I prefered to let the code here in case it is needed in the future
# move all the files to a tmp dir so lcov can access all of them
#TMPDIR=$OUTPUT_NAME.tmp
#mkdir -p ./tests/gcoverage/$TMPDIR
#cd ./tests/gcoverage/$TMPDIR
#find ../../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;
#find ../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;

# generate the HTML. It MUST be done from the src/ directory
# It needs the following 2 steps
lcov  -d . -c -o tests/gcoverage/$OUTPUT_DIR/$LCOV_OUTPUT_NAME
genhtml -t "Blockem Tests Code Coverage" -o tests/gcoverage/$OUTPUT_DIR/ tests/gcoverage/$OUTPUT_DIR/$LCOV_OUTPUT_NAME

