target_sources(
    ${PROJECT_NAME} PUBLIC
    test_co2_sensor.cpp
    test_temperature_sensor.cpp
    test_humidity_sensor.cpp
    test_event_group_await_bits.cpp
    ../include/FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
    test_controller_receiver.cpp
 
)