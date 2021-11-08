#ifndef __CONFIG_CONFIG_CL_CXY_SANSUI_P300_H__
#define __CONFIG_CONFIG_CL_CXY_SANSUI_P300_H__


//OTA
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          1
#define OTA_VERSION_STRING          "1.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/cl-cxy-speaker-sansui-p300/cfg.txt"


//WIFI
#define WIFI_SSID           "HUAWEI-House_2.4G"
#define WIFI_PAWD           "like91.nd"

//MQTT
#define MQTT_IP                     "172.24.140.41"
#define MQTT_PORT                   1883
#define MQTT_USER                   "device"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/"
#define MQTT_PUB_TOPIC              "cmdgate/central/cmd" 

#endif //