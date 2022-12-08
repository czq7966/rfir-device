#ifndef CONFIG_COM_PENET_Z3_H
#define CONFIG_COM_PENET_Z3_H

#include "com-penet.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "COM-PENET-Z3"

//Serial
#undef  SERIAL_DEBUG
#undef  SERIAL_BAUD
#define SERIAL_DEBUG                false
#define SERIAL_BAUD                 1152


#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 5
#ifdef ESP8266
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp8266/com-penet-z3/cfg.txt"
#endif
#ifdef CONFIG_IDF_TARGET_ESP32C3
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp32c3/com-penet-z3/cfg.txt"
#endif

#endif