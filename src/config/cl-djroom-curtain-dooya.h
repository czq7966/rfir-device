#ifndef __CONFIG_CONFIG_CL_VR_F1_AC_OKONOFF_H__
#define __CONFIG_CONFIG_CL_VR_F1_AC_OKONOFF_H__

//Other
#define DEBUG_RFIR

//Serial
#define SERIAL_BAUD                 9600
#define SERIAL_CONFIG               SERIAL_8N1

//OTA
// #define DISABLE_OTA                 TRUE
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          1
#define OTA_VERSION_STRING          "1.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/cl-djroom-curtain-dooya/cfg.txt"


//WIFI
// #define DISABLE_WIFI                TRUE
#define WIFI_SSID                   {"MERCURY_95E8", "ND-MAC"}
#define WIFI_PASSWORD               {"12345678", "wanglong"}
#define WIFI_RESET_TIMEOUT          {30, 30}  //多少时间内WIFI未连接，重启，单位秒

//AP
#define AP_MODE                     false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASSWORD                 "12345678"
#define AP_RESET_TIMEOUT            60 * 5 //多少时间内WIFI未连接，重启，单位秒
#define AP_CONFIG_VERSION           "0" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位秒

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