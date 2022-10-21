#ifndef CONFIG_BASE_
#define CONFIG_BASE_

#include "jled.h"

#define CHIP_ID_PREFIX              "ndiot" //设备ID前缀
#define CHIP_ID_USE_MAC             true //使用MAC地址作为设备ID

#define CFG_VERSION                 1  //配置版本号，与本地配置版本号对比，哪个高用哪个配置；-1：不使用本地配置

//Device
#define DEV_VENDOR                  "ND"
#define DEV_MODEL                   "BASE"
#define DEV_ADDRESS                 0


//LED
#define PIN_LED                     BUILTIN_LED
//BUTTON
#define PIN_BUTTON                     0
//RESET
#define PIN_RESET                   12

//Serial
#define SERIAL_DEBUG                false
#define SERIAL_BAUD                 96
#define SERIAL_DATA                 8
#define SERIAL_SUM                  0
#define SERIAL_STOP                 1
#define SERIAL_STREAM               0
#define SERIAL_READ_TIMEOUT         10
#define SERIAL_READ_BUFSIZE         1024 * 2
#define SERIAL_HALF_RW_PIN          5
#define SERIAL_HALF_R_LEVEL         0

//OTA
#define OTA_DISABLE                 false
#define OTA_VERSION                 1
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/base/cfg.txt"
#define OTA_UPDATE_INTERVAL         60 * 24     //24小时检查一次OTA,单位：分钟

//WIFI
#define WIFI_DISABLE                false
#define WIFI_SSID_DEV               ""
#define WIFI_PASS_DEV               ""
#define WIFI_SSID                   ""
#define WIFI_PASS                   ""
#define WIFI_RESET_TIMEOUT          60 * 10 //多少时间内WIFI未连接，重启，单位秒
#define WIFI_CONNECT_JLED           GJLed.Stop().LowActive().Blink(500, 500).Forever().Reset()


//AP
#define AP_DISABLE                  false //true时，支持热点配网
#define AP_SSID                     ""  //为空时，取ChipID
#define AP_PASS                     "12345678"
#define AP_START_TIMEOUT            60 //WIFI断网多少秒，启动热点配网
#define AP_CONFIG_VERSION           "1" //该值若有修改，AP配置将被重置
#define AP_CONFIG_JLED              GJLed.Stop().LowActive().Blink(10000, 500).Forever().Reset()

//MQTT
#define MQTT_DISABLE                false
#define MQTT_IP                     "push-access.sdp.101.com"
#define MQTT_PORT                   1780
#define MQTT_USER                   "62f36682a30e9400102f4f23"
#define MQTT_PASS                   ""
#define MQTT_KEEPALIVE              15  //单位秒
#define MQTT_CLEAN_SESSION          false
#define MQTT_RESET_TIMEOUT          60  //MQTT连接失败超时后，自动重启硬件, 单位秒
#define MQTT_CONNECT_JLED           GJLed.Stop().LowActive().Blink(1500, 1500).Forever().Reset()

//MQTT Topic
#define MQTT_SUB_TOPIC                "0/0/0/0/dev/{dev}/0/0"
#define MQTT_PUB_TOPIC                "0/0/dev/{dev}/0/0/0/0"

#define MQTT_RESP_TIMEOUT           10 //毫秒
#define MQTT_BUFFER_SIZE            1024 * 3

//Timer Report 
#define NET_REPORT_TIMEOUT          60   //1分钟上报一次
#define NET_HANDSHAKE_TIMEOUT       60 * 5 //5分钟握一次手 
#define NET_RESET_TIMEOUT           60 * 2 //2分钟组网超时


#endif //