cmake_minimum_required(VERSION 3.14)
project(firmware.elf)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VERBOSE ON)

add_executable( ${PROJECT_NAME} )

# Silly but we need to clear defaults before setting the right values
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_MINSIZEREL "")

# Common compile options
set(COMPILE_OPTIONS_COM -funsigned-char -funsigned-bitfields -DF_CPU=16000000L -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -Wall -mmcu=atmega2560 -x c -std=gnu99 -MP)
# Compile options for RELEASE build
set(COMPILE_OPTIONS_REL ${COMPILE_OPTIONS_COM} -Os -DNDEBUG)
# Compile options for DEBUG build
set(COMPILE_OPTIONS_DBG ${COMPILE_OPTIONS_COM} -g2 -Os -DDEBUG)
# Compile options for RELEASE W. DEBUG build
set(COMPILE_OPTIONS_RDB ${COMPILE_OPTIONS_COM} -g2 -Os -DDEBUG)
# Compile options for RELEASE MINIMAL SIZE 
set(COMPILE_OPTIONS_MIN ${COMPILE_OPTIONS_COM} -Os -DDEBUG)

# Map compile options according to build configuration
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:RELEASE>: ${COMPILE_OPTIONS_REL}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:DEBUG>: ${COMPILE_OPTIONS_DBG}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:RELWITHDEBINFO>: ${COMPILE_OPTIONS_RDB}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:MINSIZEREL>: ${COMPILE_OPTIONS_MIN}>)

target_include_directories(${PROJECT_NAME} PUBLIC drivers/include)
target_link_libraries(${PROJECT_NAME} IotLibrary -Wl,-Map=${PROJECT_NAME}.map -Wl,--start-group -Wl,-lm -Wl,-lIotLibrary -Wl,--end-group -Wl,-L/workspaces/iot-demo/target/drivers/lib -Wl,--gc-sections -mrelax -mmcu=atmega2560)

add_subdirectory(src)
add_subdirectory(free-rtos)

add_custom_target(
    create_firmware_files ALL
    COMMAND /usr/bin/avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ${PROJECT_NAME} firmware.hex
    COMMAND /usr/bin/avr-objcopy -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex ${PROJECT_NAME} firmware.eep || exit 0
    COMMAND /usr/bin/avr-objdump -h -S ${PROJECT_NAME} > firmware.lss
    COMMAND /usr/bin/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ${PROJECT_NAME} firmware.srec
    COMMAND /usr/bin/avr-size ${PROJECT_NAME}
)

add_dependencies(create_firmware_files ${PROJECT_NAME})
