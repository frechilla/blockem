#!/bin/bash

echo "WARNING: You might need to update file permissions in 'debian' directory if this is a fresh checkout"

TMPDIR=`date '+tmp.%Y%m%d%M%S'`
LIST_OF_DIRS=`find debian/ -type d -not -iwholename "*\.svn*"`
LIST_OF_FILES=`find debian/ -type f -not -iwholename "*\.svn*"`

for THISDIR in $LIST_OF_DIRS; do
    mkdir -p $TMPDIR/$THISDIR
    chmod u=rwx,g=rx,o=rx $TMPDIR/$THISDIR
done

for THISFILE in $LIST_OF_FILES; do
    cp -p $THISFILE $TMPDIR/$THISFILE
done

cd $TMPDIR 
dpkg-deb --build debian ..

rm -rf $TMPDIR

