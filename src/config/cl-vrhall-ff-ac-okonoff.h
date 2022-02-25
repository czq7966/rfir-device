#ifndef __CONFIG_CONFIG_CL_VR_F1_AC_OKONOFF_H__
#define __CONFIG_CONFIG_CL_VR_F1_AC_OKONOFF_H__

//Other
#define DEBUG_RFIR
#define HLW8110_IA_Switch_Point     50

//Power
#define PWR_RESET_CHK_PIN           5
#define PWR_RESET_CHK_VALUE         1

//Serial
#define SERIAL_BAUD                 9600
#define SERIAL_CONFIG               SERIAL_8E1

//OTA
// #define DISABLE_OTA                 TRUE
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          13
#define OTA_VERSION_STRING          "13.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/cl-vrhall-ff-ac-okonoff/cfg.txt"


//WIFI
// #define DISABLE_WIFI                TRUE
#define WIFI_SSID                   {"ND-MAC", "MERCURY_95E8"}
#define WIFI_PASSWORD               {"wanglong", "12345678"}
// #define WIFI_SSID                   {"ND-Guest"}
// #define WIFI_PASSWORD               {"88085000"}
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
// #define MQTT_IP                     "172.24.140.41"
// #define MQTT_PORT                   1883
// #define MQTT_USER                   "device"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/{did}"
#define MQTT_PUB_TOPIC              "cmdgate/central/{did}" 
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒

#endif //