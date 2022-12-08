#ifndef CONFIG_COM_PENET_H
#define CONFIG_COM_PENET_H

#include "base.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "COM-PENET"

//Serial
#undef  SERIAL_READ_TIMEOUT
#define SERIAL_READ_TIMEOUT         10

#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 2
#ifdef ESP8266
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp8266/com-penet/cfg.txt"
#endif
#ifdef CONFIG_IDF_TARGET_ESP32C3
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp32c3/com-penet/cfg.txt"
#endif

#endif