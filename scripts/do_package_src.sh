TARGET=blockem
DATE=`date +"%Y%m%d-%H%M%S"`
PACKAGE_DIR=$TARGET.$DATE
rm -rf $PACKAGE_DIR

echo "Packaging the source in package/$PACKAGE_DIR.tar.gz"

for FILE in `find .. -type f \( -not -iwholename "*.svn*" -not -iwholename "*package*" \)`
do 
    FILE_NO_DOTS=`echo $FILE | sed "s/^\.\.\///;"`
    DIR_NO_DOTS=$PACKAGE_DIR/`dirname $FILE_NO_DOTS`

    mkdir -p $DIR_NO_DOTS
    cp $FILE $DIR_NO_DOTS 
done

# copy the do_package script also
mkdir -p $PACKAGE_DIR/package
cp do_package_src.sh $PACKAGE_DIR/package/

# create the tarball
rm -rf $PACKAGE_DIR.tar.gz
tar -zcf $PACKAGE_DIR.tar.gz $PACKAGE_DIR/

