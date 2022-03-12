#ifndef __CONFIG_CONFIG_CL_DJROOM_LIGHT_MSwitch2_H__
#define __CONFIG_CONFIG_CL_DJROOM_LIGHT_MSwitch2_H__

//OTA
#define OTA_UPDATE
#define OTA_VERSION_NUMBER          1
#define OTA_VERSION_STRING          "1.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/dj-room-light-mswitch2/cfg.txt"


//WIFI
// #define WIFI_SSID                   {"HUAWEI-House_2.4G",   "djoffice-internal", "MERCURY_95E8"}
// #define WIFI_PASSWORD               {"like91.nd",           "like91.nd",         "12345678"}
#define WIFI_SSID                   {"MERCURY_95E8"}
#define WIFI_PASSWORD               {"12345678"}
#define WIFI_RESET_TIMEOUT          {30,                    30,                   30}  //多少时间内WIFI未连接，重启，单位秒
// #define WIFI_CONNECT_ON_START       false

//AP
#define AP_MODE                     false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASSWORD                 "12345678"
#define AP_RESET_TIMEOUT            60 * 5 //多少时间内WIFI未连接，重启，单位秒
#define AP_CONFIG_VERSION           "0" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位秒

//MQTT
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "ioe"
#define MQTT_PASSWORD               ""
#define MQTT_SUB_TOPIC              "cmdgate/device/{did}"
#define MQTT_PUB_TOPIC              "cmdgate/central/{did}" 
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒

//RFIR
#define DEBUG_RFIR
#define RFIR_MSWITCH_SNIFF_PIN      5

#define RFIR_MSWITCH_1_NAME         "state_l1"
#define RFIR_MSWITCH_1_CODE_TOGGLE  "0x6def7f"
#define RFIR_MSWITCH_1_PIN_IN       12
#define RFIR_MSWITCH_1_PIN_OUT      14

#define RFIR_MSWITCH_2_NAME         "state_l2"
#define RFIR_MSWITCH_2_CODE_TOGGLE  "0x6defbf"
#define RFIR_MSWITCH_2_PIN_IN       13
#define RFIR_MSWITCH_2_PIN_OUT      16

#define RFIR_MSWITCH_3_CODE_TOGGLE  ""
#define RFIR_MSWITCH_4_CODE_TOGGLE  ""
#define RFIR_MSWITCH_5_CODE_TOGGLE  ""
#define RFIR_MSWITCH_6_CODE_TOGGLE  ""
#define RFIR_MSWITCH_7_CODE_TOGGLE  ""
#define RFIR_MSWITCH_8_CODE_TOGGLE  ""

#endif //