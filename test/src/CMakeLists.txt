target_sources(
    ${PROJECT_NAME} PUBLIC
    test_co2_sensor.cpp
    test_event_group_await_bits.cpp
    /workspaces/SEP4IOT/test/include/FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
    test_controller_receiver.cpp
   
)