#ifndef __CONFIG_CONFIG_FZ_PMZ_F2_AC_GREE_H__
#define __CONFIG_CONFIG_FZ_PMZ_F2_AC_GREE_H__

#include "rfir/util/debuger.h"

// #define DEBUG_RFIR

//Device
#define DEV_FACTURER "ND"
#define DEV_MODEL "ACGREEPMZ"

//Serial
#define SERIAL_BAUD                 115200
#define SERIAL_CONFIG               SERIAL_8N1

//OTA
#define OTA_UPDATE                  true
#define OTA_VERSION_NUMBER          8
#define OTA_VERSION_STRING          "8.0"
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/fz-pmz-f2-ac-gree/cfg.txt"


//WIFI
#define WIFI_SSID_DEV               {"MERCURY_95E8"}
#define WIFI_PASSWORD_DEV           {"12345678"}
#define WIFI_SSID                   {"ND-MAC"}
#define WIFI_PASSWORD               {"wanglong"}
#define WIFI_RESET_TIMEOUT          {30, 30, 30, 30, 30}  //多少时间内WIFI未连接，重启，单位秒

//AP
#define AP_MODE                     false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASSWORD                 "12345678"
#define AP_RESET_TIMEOUT            60 * 5 //多少时间内WIFI未连接，重启，单位秒
#define AP_CONFIG_VERSION           "1" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位秒

//SMC
#define SMC_MODE                     true //true时，支持智能配网
#define SMC_CONFIG_VERSION           "0" //该值若有修改，SMC配置将被重置
#define SMC_CONFIG_FILE              "/config/smc.json"
#define SMC_CONFIG_PIN               0  //GPIO，配合SMC_CONFIG_SETTING，按键后进入智能配网模式
#define SMC_CONFIG_PIN_TYPE          0x11  //按键类型，请参考ace_button::ButtonConfig::FeatureFlagType, 如： kFeatureClick = 0x01;
#define SMC_CONFIG_PIN_NUM           3  //连续按键次数
#define SMC_CONFIG_PIN_DELAY         0  //按键Delay(秒，0默认)
#define SMC_CONFIG_PIN_INTERVAL      3  //几秒之内完成动作(秒)，如3秒之内
#define SMC_CONFIG_JLED              JLed(BUILTIN_LED).Blink(500, 5000).Forever()
#define SMC_CONFIG_TIMEOUT           5  //几分钟之内需完成配网，之外重启(分)

//MQTT
// #define DISABLE_MQTT                TRUE
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "ioe"
#define MQTT_PASSWORD               ""
#define MQTT_KEEPALIVE              60  //保活几分钟
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒
#define MQTT_SUB_TOPIC              "111ndiot/nd/device/event/report/{did}"
#define MQTT_PUB_TOPIC              "222ndiot/nd/device/event/report/{did}"

#define MQTT_DSP_SVC_LOGIN          "ndiot/nd/dispatcher/service/login/{did}"

#define MQTT_EDG_SVC_HANDSHAKE      "ndiot/nd/edge/service/handshake/{did}"

#define MQTT_DEV_SVC_LOGIN          "ndiot/nd/device/service/login/{did}"
#define MQTT_DEV_SVC_HANDSHAKE      "ndiot/nd/device/service/handshake/{did}"
#define MQTT_DEV_SVC_GET            "ndiot/nd/device/service/get/{did}"
#define MQTT_DEV_SVC_SET            "ndiot/nd/device/service/set/{did}"

#define MQTT_DEV_EVT_HEARTBEAT      "ndiot/nd/device/event/heartbeat/{did}"
#define MQTT_DEV_EVT_REBOOT         "ndiot/nd/device/event/reboot/{did}"
#define MQTT_DEV_EVT_RECONNECT     "ndiot/nd/device/event/reconnect/{did}"
#define MQTT_DEV_EVT_REPORT         "ndiot/nd/device/event/report/{did}"



#endif //