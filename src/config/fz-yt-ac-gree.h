#ifndef __CONFIG_CONFIG_FZ_YT_AC_GREE_H__
#define __CONFIG_CONFIG_FZ_YT_AC_GREE_H__

#include "rfir/util/debuger.h"
#include "jled.h"
#include "AceButton.h"

extern JLed                         GJLed;

//Device
#define DEV_VENDOR                  "ND"
#define DEV_MODEL                   "FZYTACGREE"

// #define DEBUGER RFIRDebuger
#define NETWORKING_V3

//LED
#define LED_PIN                     BUILTIN_LED
//BUTTON
#define BUTTON_PIN                  0
//RESET
#define RESET_PIN                   12

//Serial
#define SERIAL_BAUD                 115200
#define SERIAL_CONFIG               SERIAL_8N1

//COSerial
#define CO_SERIAL_BAUD              115200
#define CO_SERIAL_CONFIG            SERIAL_8N1

//OTA
#define OTA_DISABLE                 false
#define OTA_UPDATE                  true
#define OTA_VERSION_NUMBER          5
#define OTA_VERSION_STRING          "5.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/fz-yt-ac-gree/cfg.txt"
#define OTA_UPDATE_INTERVAL         1000 * 60 * 60 * 3     //3小时检查一次OTA

//WIFI
#define WIFI_DISABLE                false
#define WIFI_SSID_DEV               {"MERCURY_95E8"}
#define WIFI_PASSWORD_DEV           {"12345678"}
#define WIFI_SSID                   {"NDSEC", "ND-MAC"}
#define WIFI_PASSWORD               {"wanglong","wanglong"}
// #define WIFI_RESET_TIMEOUT          {300, 300, 300, 300, 300}  //多少时间内WIFI未连接，重启，单位秒
#define WIFI_RESET_TIMEOUT          1000 * 60  //多少时间内WIFI未连接，重启，单位毫秒
#define WIFI_CONNECT_JLED           GJLed.Stop().LowActive().Blink(500, 500).Forever().Reset()


//AP
#define AP_MODE                     false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASSWORD                 "12345678"
#define AP_RESET_TIMEOUT            1000 * 60 * 5 //多少时间内WIFI未连接，重启，单位毫秒
#define AP_CONFIG_VERSION           "1" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       1000 * 5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位毫秒

//SMC
#define SMC_DISABLE                  false
#define SMC_CONFIG_VERSION           "0" //该值若有修改，SMC配置将被重置
#define SMC_CONFIG_FILE              "/config/smc.json"
#define SMC_CONFIG_PIN               BUTTON_PIN  //GPIO，配合SMC_CONFIG_SETTING，按键后进入智能配网模式
#define SMC_CONFIG_PIN_TYPE          0x11  //按键类型，请参考ace_button::ButtonConfig::FeatureFlagType, 如： kFeatureClick = 0x01;
#define SMC_CONFIG_PIN_NUM           3  //连续按键次数
#define SMC_CONFIG_PIN_DELAY         0  //按键Delay(秒，0默认)
#define SMC_CONFIG_PIN_INTERVAL      3  //几秒之内完成动作(秒)，如3秒之内
#define SMC_CONFIG_JLED              GJLed.Stop().LowActive().Blink(500, 500).Forever()

#define SMC_CONFIG_TIMEOUT           5  //几分钟之内需完成配网，之外重启(分)

//MQTT
#define MQTT_DISABLE                false
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#ifdef NETWORKING_V3
    #define MQTT_USER                   "south"
#else
    #define MQTT_USER                   "ioe"
#endif
#define MQTT_PASSWORD               ""
#define MQTT_KEEPALIVE              15  //单位秒
#define MQTT_CLEAN_SESSION          false
#define MQTT_RESET_TIMEOUT          1000 * 60  //MQTT连接失败超时后，自动重启硬件, 单位毫秒
#define MQTT_CONNECT_JLED           GJLed.Stop().LowActive().Blink(1500, 1500).Forever().Reset()

//MQTT Topic


//DSP.sub
#define MQTT_DSP_EVT_STATUS         "0/0/dsp/{dsp}/0/0/evt/status"

//EDG.sub
#define MQTT_EDG_EVT_STATUS         "0/0/edg/{edg}/0/0/evt/status"

//DEV.sub
#define MQTT_DEV_SVC_LOGIN          "{app}/{dom}/+/+/dev/{dev}/svc/login"
#define MQTT_DEV_SVC_HANDSHAKE      "{app}/{dom}/edg/{edg}/dev/{dev}/svc/handshake"
#define MQTT_DEV_SVC_PENET          "{app}/{dom}/edg/{edg}/dev/{dev}/svc/penet"
#define MQTT_DEV_SVC_GET            "{app}/{dom}/+/+/dev/{dev}/svc/get"
#define MQTT_DEV_SVC_SET            "{app}/{dom}/+/+/dev/{dev}/svc/set"
#define MQTT_DEV_SVC_REBOOT         "{app}/{dom}/+/+/dev/{dev}/svc/reboot"



#define MQTT_RESP_TIMEOUT           1000 * 10 //毫秒
#define MQTT_BUFFER_SIZE            2048
//Timer Report 
#define DEVICE_TIMER_REPORT_TIMEOUT        1000 * 60   //1分钟上报一次
#define NETWORKING_RE_HANDSHAKE_TIMEOUT    1000 * 60 * 5 //5分钟握一次手 
#define NETWORKING_RESET_TIMEOUT    1000 * 60 * 2 //2分钟组网超时
#define NETWORKING_LOGIN_JLED       GJLed.Stop().LowActive().Blink(2500, 500).Forever().Reset()

//NETWORKING V3
#define MQTT_DEV_SUB                "+/+/+/+/dev/{dev}/+/+"
#define MQTT_DEV_PUB                "0/0/dev/{dev}/0/0/0/0"
#define MQTT_DEV_STATUS             "0/0/dev/{dev}/0/0/evt/status"

#endif //