#!/bin/bash

############################################################################
# This script generates html-based code coverage report in the current     #
# path in a directory called gcovYYYYMMDDHHSS. You can browse the report   #
# opening the index.html file stored in that path                          #
# This script doesn't take any parameters by command line                  #
#                                                                          #
# This script calculates the coverage of:                                  #
#   1) The blockemtest app (in $blockem-path/src/tests/blockemtest)        #
#   2) A few tests on the command line options to ensure everything works  #
#      as expected                                                         #
#                                                                          #
# IMPORTANT: Make sure you compiled blockem with support for gcov. This is #
# done running the configure script with the following parameter:          #
#     './configure --enable-gcov'                                          #
# You'll get better code coverage results enabling debugging code too      #
############################################################################

if [ $# -eq 1 ]; then
    if [ $1 = "--help" ]; then
        echo "This script generates html-based code coverage report in the current"
        echo "path in a directory called gcovYYYYMMDDHHSS. You can browse the report"
        echo "opening the index.html file stored in that path"
        echo "This script doesn't take any parameters by command line"
        echo ""
        echo "This script calculates the coverage of: "
        echo "  1) The blockemtest app (in $blockem-path/src/tests/blockemtest)"
        echo "  2) A few tests on the command line options to ensure everything works"
        echo "     as expected"
        echo ""
        echo "IMPORTANT: Make sure you compiled blockem with support for gcov. This is"
        echo "done running the configure script with the following parameter:"
        echo "    './configure --enable-gcov'"
        echo "You'll get better code coverage results enabling debugging code too"
    else
        echo "bad parameter. Try '$0 --help'"
    fi
    
    exit 0
fi

OUTPUT_DIR=gcov`date +%Y%m%d%H%M%S`
GCOV_OUTPUT_NAME=blockem.gcov
LCOV_OUTPUT_NAME=blockem.lcov

#cd into base directory
cd ../

# test if the binaries exist
if [ ! -f src/blockem ]; then
    echo "You must compile blockem before launching the code coverage tool. Run '$0 --help' for more help"
	exit
fi
if [ ! -f src/tests/blockemtest ] ; then
    echo "You must compile blockemtest before launching the code coverage tool. Run '$0 --help' for more help"
	exit
fi

# lcov is required to generate the fancy looking html
type lcov &> /dev/null ||  { echo "lcov is required but it's not installed. Aborting." >&2; exit 1; }

# reset lcov counters
lcov --directory src/ --zerocounters
lcov --directory src/tests/ --zerocounters

# apps must be run from their directories or counters won't be updated properly
# firstly run the blockem test application
cd src/tests
echo "running blockem test application in src/tests"
./blockemtest

# cd back into the blockem directory (not the test app directory)
cd ../

# now test the command line options
# help & version message
./blockem --help
./blockem --version
# parsing command line error
./blockem --force-parsing-error
# bad mode option
./blockem --mode=3
# several errors due to incompatible command line options
# number of columns missing
./blockem --mode=1 -r 2
# invalid amount of rows
./blockem --mode=1 -r 0 -c 12
# only starting column is set
./blockem --mode=1 -r 14 -c 14 --starting-column=4
# trying to start a game outside the board boundaries
./blockem --mode=1 -r 14 -c 14 --starting-column=4 --starting-row=14
# trying to start a game from a negative column
./blockem --mode=1 -r 14 -c 14 --starting-column=-1 --starting-row=13
# trying to start a 1vs1 game without a game file
./blockem --mode=2
# invalid heuristic type
./blockem --mode=2 --heuristic=-1 tests/examples/games/game.txt
# depth parameter missing
./blockem --mode=2 tests/examples/games/game.txt
# invalid depth
./blockem --mode=2 --depth=-1 tests/examples/games/game.txt
# game file does not exist
./blockem --mode=2 --depth=2 this-file-does-not-exist



# run gcov through all the files generated when running the apps
# gcov must be run from the directory where the app is created by the makefile
mkdir -p ../scripts/$OUTPUT_DIR
# files in the 'tests' directory
cd tests/
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov >> ../../scripts/$OUTPUT_DIR/$GCOV_OUTPUT_NAME
# now the src/ dir
cd ../
find . -maxdepth 1 -not -wholename "*\.svn*" -type f -name "*.gcda" | xargs gcov > ../scripts/$OUTPUT_DIR/$GCOV_OUTPUT_NAME


## NOTE this is not needed, though I prefered to let the code here in case it is needed in the future
# move all the files to a tmp dir so lcov can access all of them
#TMPDIR=$OUTPUT_NAME.tmp
#mkdir -p ./tests/gcoverage/$TMPDIR
#cd ./tests/gcoverage/$TMPDIR
#find ../../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;
#find ../../ -maxdepth 1 -type f \( -name "*.gcov" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.h" -o -name "*.cpp" \) -exec cp '{}' . \;

# generate the HTML. It MUST be done from the src/ directory
# It needs the following 2 steps
lcov  -d . -c -o ../scripts/$OUTPUT_DIR/$LCOV_OUTPUT_NAME
genhtml -t "Blockem Tests Code Coverage" -o ../scripts/$OUTPUT_DIR/ ../scripts/$OUTPUT_DIR/$LCOV_OUTPUT_NAME
