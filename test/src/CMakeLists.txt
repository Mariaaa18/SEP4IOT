target_sources(
    ${PROJECT_NAME} PUBLIC
    test_co2_sensor.cpp
    test_temperature_sensor.cpp
    test_humidity_sensor.cpp
    test_event_group_await_bits.cpp
    /workspaces/SEP4IOT/test/include/FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
)