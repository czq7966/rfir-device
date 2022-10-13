#ifndef CONFIG_BASE_
#define CONFIG_BASE_

#include "jled.h"

#define CFG_VERSION          1  //配置版本号，与本地配置版本号对比，哪个高用哪个配置；-1：不使用本地配置

//Device
#define DEV_VENDOR                  "ND"
#define DEV_MODEL                   "RS485PENET"


//LED
#define PIN_LED                     BUILTIN_LED
//BUTTON
#define PIN_BUTTON                     0
//RESET
#define PIN_RESET                   12

//Serial
#define SERIAL_DEBUG                false
#define SERIAL_BAUD                 9600
#define SERIAL_DATA                 8
#define SERIAL_SUM                  0
#define SERIAL_STOP                 1
#define SERIAL_STREAM               0

//OTA
#define OTA_DISABLE                 false
#define OTA_UPDATE                  true
#define OTA_VERSION                 10
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/rs485-penet/cfg.txt"
#define OTA_UPDATE_INTERVAL         1000 * 60 * 60 * 3     //3小时检查一次OTA

//WIFI
#define WIFI_DISABLE                false
#define WIFI_SSID_DEV               "MERCURY_95E8"
#define WIFI_PASS_DEV               "12345678"
#define WIFI_SSID                   "NDSEC ND-MAC"
#define WIFI_PASS                   "wanglong wanglong"
#define WIFI_RESET_TIMEOUT          1000 * 60 * 10 //多少时间内WIFI未连接，重启，单位毫秒
#define WIFI_CONNECT_JLED           GJLed.Stop().LowActive().Blink(500, 500).Forever().Reset()


//AP
#define AP_DISABLE                  true //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASS                     "12345678"
#define AP_RESET_TIMEOUT            1000 * 60 * 5 //多少时间内WIFI未连接，重启，单位毫秒
#define AP_START_TIMEOUT            1000 * 20 //WIFI断网多少毫秒，启动热点配网
#define AP_CONFIG_VERSION           "1" //该值若有修改，AP配置将被重置
#define AP_CONFIG_PIN               -1  //配合AP_CONFIG_PIN_TIMEOUT，长按后进入AP配网模式
#define AP_CONFIG_PIN_TIMEOUT       1000 * 5   //配合AP_CONFIG_PIN，长按后进入AP配网模式，单位毫秒
#define AP_CONFIG_JLED             GJLed.Stop().LowActive().Blink(5000, 500).Forever().Reset()

//MQTT
#define MQTT_DISABLE                false
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "south"
#define MQTT_PASS                   ""
#define MQTT_KEEPALIVE              15  //单位秒
#define MQTT_CLEAN_SESSION          false
#define MQTT_RESET_TIMEOUT          1000 * 60  //MQTT连接失败超时后，自动重启硬件, 单位毫秒
#define MQTT_CONNECT_JLED           GJLed.Stop().LowActive().Blink(1500, 1500).Forever().Reset()

//MQTT Topic
#define MQTT_SUB_TOPIC                "0/0/0/0/dev/{dev}/0/0"
#define MQTT_PUB_TOPIC                "0/0/dev/{dev}/0/0/0/0"

#define MQTT_RESP_TIMEOUT           1000 * 10 //毫秒
#define MQTT_BUFFER_SIZE            1024 * 3

//Timer Report 
#define NET_REPORT_TIMEOUT        1000 * 60   //1分钟上报一次
#define NET_HANDSHAKE_TIMEOUT    1000 * 60 * 5 //5分钟握一次手 
#define NET_RESET_TIMEOUT    1000 * 60 * 2 //2分钟组网超时


#endif //