#ifndef CONFIG_RFIR_PENET_ND_AC_H
#define CONFIG_RFIR_PENET_ND_AC_H

#include "rfir-penet.h"

#define CHIP_ID_PREFIX              "ESP8266x" //设备ID前缀
#define CHIP_ID_USE_MAC             false //使用MAC地址作为设备ID

#undef  DEV_MODEL
#define DEV_MODEL                   "RFIR-PENET-ND-AC"

//Reset
#undef  PIN_RESET
#define PIN_RESET                   12

//WIFI
#undef  WIFI_SSID_DEV
#undef  WIFI_PASS_DEV
#undef  WIFI_SSID
#undef  WIFI_PASS
#define WIFI_SSID_DEV               "MERCURY_95E8"
#define WIFI_PASS_DEV               "12345678"
#define WIFI_SSID                   "NDSEC,ND-MAC"
#define WIFI_PASS                   "wanglong,wanglong"


//OTA
#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#undef  OTA_REBOOT_AFTER_CHECK
#define OTA_VERSION                 29
#ifdef ESP8266
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp8266/rfir-penet-nd-ac/cfg.txt"
#endif
#ifdef CONFIG_IDF_TARGET_ESP32C3
    #define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/esp32c3/rfir-penet-nd-ac/cfg.txt"
#endif
#define OTA_REBOOT_AFTER_CHECK      true

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