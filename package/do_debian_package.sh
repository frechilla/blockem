#!/bin/bash

#############################################################################
# This script generates a debian package (.deb) which contains blockem.     #
# It retrieves the binary blockem file from '../src/blockem' and copies it  #
# in the directory 'debian' (which resides in this directory). It then      #
# calculates the md5sum to update the md4sums file of the debian package    #
# and calls the expected utilities to build it.                             #
#                                                                           #
# IMPORTANT: make sure you've run the script 'configure' with the following #
# options before building blockem (running make). Otherwise the app won't   #
# work properly once the debian package is installed                        #
#   './configure --localedir="/usr/share/locale" --datadir="/usr/share"'    #
#############################################################################

if [ $# -eq 1 ]; then
    if [ $1 = "--help" ]; then
        echo "This script generates a debian package (.deb) which contains blockem."
        echo "It retrieves the binary blockem file from '../src/blockem' and copies it"
        echo "in the directory 'debian' (which resides in this directory). It then"
        echo "calculates the md5sum to update the md5sums file of the debian package"
        echo "and calls the expected utilities to build it."
        echo ""
        echo "IMPORTANT: make sure you've run the script 'configure' with the following"
        echo "options before building blockem (running make). Otherwise the app won't"
        echo "work properly once the debian package is installed "
        echo "  './configure --localedir="/usr/share/locale" --datadir="/usr/share" --bindir="/usr/bin"'"
    else
        echo "bad parameter. Try '$0 --help'"
    fi

    exit 0
fi

if [ ! -f ../src/blockem ] 
then
	echo "You must compile blockem before packaging it. Run '$0 --help' for more help"
	exit
fi

TMPDIR=`date '+tmp.%Y%m%d%M%S'`
LIST_OF_DIRS=`find debian/ -type d -not -iwholename "*\.svn*"`
LIST_OF_FILES=`find debian/ -type f -not -iwholename "*\.svn*"`

# create directories without .svn stuff
for THISDIR in $LIST_OF_DIRS; do
    mkdir -p $TMPDIR/$THISDIR
    chmod u=rwx,g=rx,o=rx $TMPDIR/$THISDIR
done

# copy all the files one by one into the new directory
for THISFILE in $LIST_OF_FILES; do
    cp -p $THISFILE $TMPDIR/$THISFILE
    chmod u=wr,g=r,o=r $TMPDIR/$THISFILE
done

# copy the brand new binary into the tmp dir and apply rights
cp -f ../src/blockem $TMPDIR/debian/usr/bin
chmod u=rwx,g=rx,o=rx $TMPDIR/debian/usr/bin/blockem

# update the md5sums file
cd $TMPDIR/debian
cat ../../debian/DEBIAN/md5sums | sed -e "s|^[0123456789abcdef]\{32\}  usr\/bin\/blockem|`md5sum usr/bin/blockem`|g" > DEBIAN/md5sums

# update total size of blockem after installation
TOTAL_SIZE=`find ./usr/ -type f -exec ls -l '{}' \; | awk 'BEGIN{total=0} {total+=$5} END{print total}'`
let TOTAL_SIZE=$TOTAL_SIZE/1024
cat ../../debian/DEBIAN/control | sed -e "s|^Installed-Size: [0123456789]*|Installed-Size: $TOTAL_SIZE|g" > DEBIAN/control

# build the package
cd ../..
dpkg-deb --build $TMPDIR/debian .

# cleaning up ...
rm -rf $TMPDIR

