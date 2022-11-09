#ifndef CONFIG_RFIR_PENET_8285_H
#define CONFIG_RFIR_PENET_8285_H

#include "rfir-penet.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "RFIR-PENET-8285"

//LED
#undef  PIN_LED
#define PIN_LED                     2

//Serial
#undef  SERIAL_DEBUG
#undef  SERIAL_READ_BUFSIZE
#define SERIAL_DEBUG                true
#define SERIAL_READ_BUFSIZE         128

//WIFI
#undef  WIFI_SSID_DEV
#undef  WIFI_PASS_DEV
#define WIFI_SSID_DEV               "TP-LINK_23#304"
#define WIFI_PASS_DEV               "czq7894561230"

//MQTT
#undef  MQTT_BUFFER_SIZE
#define MQTT_BUFFER_SIZE            1024

//OTA
#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 1
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/rfir-penet-8285/cfg.txt"

//RFIR SNIFF
#undef  RFIR_SNIFF_ENABLE   	    
#undef  RFIR_SNIFF_PIN	            
#define RFIR_SNIFF_ENABLE   	    false
#define RFIR_SNIFF_PIN	            14


//RFIR SEND
#undef  RFIR_SEND_ENABLE   	    
#undef  RFIR_SEND_PIN	
#define RFIR_SEND_ENABLE            true
#define RFIR_SEND_PIN               4


#endif