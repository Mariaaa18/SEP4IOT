#! /bin/sh
set -e

cd test
rm -rf build test_result.xml ../test_result.xml
/usr/bin/cmake -Wno-dev --no-warn-unused-cli -DDOWNLOAD_EXTRACT_TIMESTAMP=TRUE -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ -S. -Bbuild
/usr/bin/cmake --build build --config Debug --target all --

./build/iot_demo_test --gtest_output="xml:temp.xml"
sed 's/\/workspaces\/iot-base\///g' < temp.xml > ../test_result.xml
rm temp.xml
#ctest --output-junit ../test_result.xml --test-dir build
exit 0
