#ifndef __CONFIG_H__
#define __CONFIG_H__

//OTA
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          28
#define OTA_VERSION_STRING          "28.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/dj-ir-ac/cfg.txt"


//WIFI
// #define VR_ROOM_WIFI_SSID       "ND-Guest"
// #define VR_ROOM_WIFI_PAWD       "88085000"
#define DJ_ROOM_WIFI_SSID           "HUAWEI-House_2.4G"
#define DJ_ROOM_WIFI_PAWD           "like91.nd"

//MQTT
#define MQTT_IP                     "172.24.140.41"
#define MQTT_PORT                   1883
#define MQTT_USER                   "device"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/"
#define MQTT_PUB_TOPIC              "cmdgate/central/cmd" 

//SYSTEM
#define CHIP_ID_PREFIX              "ESP8266_" 

#endif