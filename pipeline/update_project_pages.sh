#! /bin/sh
set -e

# Update firmware.zip
rm -f proj_pages/firmware/firmware.zip
zip proj_pages/firmware/firmware.zip firmware.*

# Update test_result.xml
cp -f test_result.xml proj_pages/testdata/test_result.xml

# Update date stamps
NOW=$(date)
sed "s/TTTTTTTTTTTTTT/${NOW}/g" < proj_pages/index.html.template > index.html
mv -f index.html proj_pages/index.html

exit 0
