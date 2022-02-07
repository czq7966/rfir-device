#ifndef __CONFIG_CONFIG_CL_DJ_MCQUAY_H__
#define __CONFIG_CONFIG_CL_DJ_MCQUAY_H__

#define DEBUG_RFIR
#define PWR_RESET_CHK_PIN           5

//OTA
// #define DISABLE_OTA                 TRUE
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          2
#define OTA_VERSION_STRING          "2.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/cl-vrhall-ff-ac-okonoff/cfg.txt"


//WIFI
// #define DISABLE_WIFI                TRUE
#define WIFI_SSID                   {"MERCURY_95E8", "ND-Guest"}
#define WIFI_PASSWORD               {"12345678", "88085000"}
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
#define MQTT_IP                     "172.24.140.41"
#define MQTT_PORT                   1883
#define MQTT_USER                   "device"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/{did}"
#define MQTT_PUB_TOPIC              "cmdgate/central/{did}" 

#endif //