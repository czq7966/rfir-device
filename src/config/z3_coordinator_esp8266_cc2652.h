#ifndef __CONFIG_CONFIG_Z3_COORDINATOR_ESP8266_CC2652_H__
#define __CONFIG_CONFIG_Z3_COORDINATOR_ESP8266_CC2652_H__

#include "rfir/util/debuger.h"

//Device
#define DEV_FACTURER "ND"
#define DEV_MODEL "Z3CO2652ESP8266"

// #define DEBUG_RFIR

// #define DEBUGER RFIRDebuger
#define COSerial Serial

//LED
#define LED_PIN                     BUILTIN_LED
//BUTTON
#define BUTTON_PIN                  0

//Serial
#define SERIAL_BAUD                 115200
#define SERIAL_CONFIG               SERIAL_8N1

//COSerial
#define CO_SERIAL_BAUD                 115200
#define CO_SERIAL_CONFIG               SERIAL_8N1

//OTA
#define DISABLE_OTA                 TRUE
#define OTA_UPDATE                  true
#define OTA_VERSION_NUMBER          1
#define OTA_VERSION_STRING          "1.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/z3_coordinator_esp8266_cc2652/cfg.txt"


//WIFI
#define WIFI_SSID_DEV               {"MERCURY_95E8"}
#define WIFI_PASSWORD_DEV           {"12345678"}
#define WIFI_SSID                   {"MERCURY_95E8"}
#define WIFI_PASSWORD               {"12345678"}
#define WIFI_RESET_TIMEOUT          {300, 300, 300, 300, 300}  //多少时间内WIFI未连接，重启，单位秒
#define WIFI_CONNECT_JLED           JLed(LED_PIN).LowActive().Blink(500, 500).Forever()


//AP
#define AP_MODE                     false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASSWORD                 "12345678"
#define AP_RESET_TIMEOUT            60 * 5 //多少时间内WIFI未连接，重启，单位秒
#define AP_CONFIG_VERSION           "1" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位秒

//SMC
// #define DISABLE_SMC                  TRUE
#define SMC_CONFIG_VERSION           "0" //该值若有修改，SMC配置将被重置
#define SMC_CONFIG_FILE              "/config/smc.json"
#define SMC_CONFIG_PIN               BUTTON_PIN  //GPIO，配合SMC_CONFIG_SETTING，按键后进入智能配网模式
#define SMC_CONFIG_PIN_TYPE          0x11  //按键类型，请参考ace_button::ButtonConfig::FeatureFlagType, 如： kFeatureClick = 0x01;
#define SMC_CONFIG_PIN_NUM           3  //连续按键次数
#define SMC_CONFIG_PIN_DELAY         0  //按键Delay(秒，0默认)
#define SMC_CONFIG_PIN_INTERVAL      3  //几秒之内完成动作(秒)，如3秒之内
#define SMC_CONFIG_JLED              JLed(LED_PIN).Blink(500, 5000).Forever()
#define SMC_CONFIG_TIMEOUT           5  //几分钟之内需完成配网，之外重启(分)


//MQTT
// #define DISABLE_MQTT                TRUE
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "ioe"
#define MQTT_PASSWORD               ""
#define MQTT_KEEPALIVE              1  //单位秒
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒

//MQTT Topic
#define MQTT_APP                    "ndiot"
#define MQTT_DOMAIN                 "nd"

//DSP.pub
#define MQTT_DSP_SVC_LOGIN          "login"
//DSP.sub
#define MQTT_DSP_EVT_STATUS         "{app}/{dom}/dsp/{dsp}/0/0/evt/status"

//EDG.pub
#define MQTT_EDG_SVC_HANDSHAKE      "handshake"
//EDG.sub
#define MQTT_EDG_EVT_STATUS         "{app}/{dom}/edg/{edg}/0/0/evt/status"

//DEV.pub
#define MQTT_DEV_EVT_REPORT         "report"
#define MQTT_DEV_EVT_STATUS         "status"
#define MQTT_DEV_EVT_PENET          "penet"
//DEV.sub
#define MQTT_DEV_SVC_LOGIN          "{app}/{dom}/dsp/{dsp}/dev/{dev}/svc/login"
#define MQTT_DEV_SVC_HANDSHAKE      "{app}/{dom}/edg/{edg}/dev/{dev}/svc/handshake"
#define MQTT_DEV_SVC_GET            "{app}/{dom}/edg/{edg}/dev/{dev}/svc/get"
#define MQTT_DEV_SVC_SET            "{app}/{dom}/edg/{edg}/dev/{dev}/svc/set"
#define MQTT_DEV_SVC_PENET          "{app}/{dom}/edg/{edg}/dev/{dev}/svc/penet"



#define MQTT_RESP_TIMEOUT           10*1000 //毫秒
//Timer Report 
#define DEVICE_TIMER_REPORT_TIMEOUT        60*1000   

#endif //