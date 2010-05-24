#!/bin/bash

if [ ! -f ../src/blockem ] 
then
	echo "You must compile blockem before packaging it man..."
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
cp -f ../src/blockem $TMPDIR/debian/usr/local/bin
chmod u=rwx,g=rx,o=rx $TMPDIR/debian/usr/local/bin/blockem

# update the md5sums file
cd $TMPDIR/debian
cat DEBIAN/md5sums | sed -e "s|^[0123456789abcdef]\{32\}  usr\/local\/bin\/blockem|`md5sum usr/local/bin/blockem`|g" > DEBIAN/md5sums

# build the package
cd ../..
dpkg-deb --build $TMPDIR/debian .

# cleaning up ...
rm -rf $TMPDIR

