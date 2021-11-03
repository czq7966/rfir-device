#ifndef __CONFIG_CONFIG_FZ_DM4_GREE_H__
#define __CONFIG_CONFIG_FZ_DM4_GREE_H__

//OTA
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          28
#define OTA_VERSION_STRING          "28.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/fz-dm4-gree/cfg.txt"


//WIFI
#define WIFI_SSID           "MERCURY_95E8"
#define WIFI_PAWD           ""

//MQTT
#define MQTT_IP                     "172.24.140.41"
#define MQTT_PORT                   1883
#define MQTT_USER                   "device"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/"
#define MQTT_PUB_TOPIC              "cmdgate/central/cmd" 

#endif //