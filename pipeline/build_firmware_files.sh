#! /bin/sh
set -e

rm -f firmware.*
cd target
rm -rf build
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_VERBOSE_MAKEFILE:STRING=ON -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/avr-gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/avr-g++ -S. -Bbuild
/usr/bin/cmake --build build --config Release --target create_firmware_files --
mv build/firmware.* ..
exit 0

