
target_sources(${PROJECT_NAME}
    PUBLIC
    main.c
    LoRaWANHandler.c
    models/cotwo.c
    controllers/controllerSenderImpl.c
    controllers/controllerReceiverImpl.c
    models/temperature.c
    models/humidity.c
    controllers/dataShared.c
    
)