#! /bin/sh

cd test
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ -S. -Bbuild
/usr/bin/cmake --build build --config Debug --target all --

./build/iot_demo_test --gtest_output="xml:temp.xml"
sed 's/\/workspaces\/iot-demo\///g' < temp.xml > test_result.xml
rm temp.xml
#ctest --output-junit ../test_result.xml --test-dir build

