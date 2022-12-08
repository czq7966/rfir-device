#ifndef CONFIG_RFIR_PENET_H
#define CONFIG_RFIR_PENET_H

#include "base.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "RFIR-PENET"


#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 1
#ifdef ESP8266
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp8266/rfir-penet/cfg.txt"
#endif
#ifdef CONFIG_IDF_TARGET_ESP32C3
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp32c3/rfir-penet/cfg.txt"
#endif

//RFIR SNIFF
#undef  RFIR_SNIFF_ENABLE   	    
#undef  RFIR_SNIFF_PIN	            
#define RFIR_SNIFF_ENABLE   	    true
#define RFIR_SNIFF_PIN	            14


//RFIR SEND
#undef  RFIR_SEND_ENABLE   	    
#undef  RFIR_SEND_PIN	
#define RFIR_SEND_ENABLE            true
#define RFIR_SEND_PIN               4


#endif