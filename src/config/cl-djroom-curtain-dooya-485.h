#ifndef __CONFIG_CONFIG_CL_DJROOM_CURTAIN_DOOYA_485_H__
#define __CONFIG_CONFIG_CL_DJROOM_CURTAIN_DOOYA_485_H__

//Other
#define DEBUG_RFIR

//Serial
#define SERIAL_BAUD                 9600
#define SERIAL_CONFIG               SERIAL_8N1

//OTA
// #define DISABLE_OTA                 TRUE
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          2
#define OTA_VERSION_STRING          "2.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/cl-djroom-curtain-dooya-485/cfg.txt"


//WIFI
// #define DISABLE_WIFI                TRUE
#define WIFI_SSID                   {"MERCURY_95E8", "HUAWEI-House_2.4G",   "djoffice-internal" }
#define WIFI_PASSWORD               {"12345678",     "like91.nd",           "like91.nd"         }
#define WIFI_RESET_TIMEOUT          {30, 30, 30}  //多少时间内WIFI未连接，重启，单位秒

//MQTT
// #define DISABLE_MQTT                TRUE
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "ioe"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/{did}"
#define MQTT_PUB_TOPIC              "cmdgate/central/{did}" 
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒

#endif //