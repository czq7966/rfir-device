#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "config-device.h"


//SYSTEM
#ifdef ESP8266
#define CHIP_ID_PREFIX              "ESP8266x" 
#else
#define CHIP_ID_PREFIX              "ESP32x" 
#endif

//福州东门4楼Gree空调
#ifdef DEVICE_FZ_DM4_TEAROOM_AC_GREE
#include "config/fz-dm4-tearoom-ac-gree.h"
#endif

//福州东门Media空调
#ifdef DEVICE_FZ_DM_AC_MEDIA
#include "config/fz-dm-ac-media.h"
#endif

//福州亚太Media空调
#ifdef DEVICE_FZ_YT_AC_MEDIA
#include "config/fz-yt-ac-media.h"
#endif

//福州亚太格力空调
#ifdef DEVICE_FZ_YT_AC_GREE
#include "config/fz-yt-ac-gree.h"
#endif

// 福州飘渺庄2楼食堂格力空调
#ifdef DEVICE_FZ_PMZ_F2_AC_GREE
#include "config/fz-pmz-f2-ac-gree.h"
#endif

//长乐DJ会议室Mcquay空调
#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "config/cl-djroom-ac-mcquay.h"
#endif

//长乐DJ会议室杜亚窗帘
#ifdef DEVICE_CL_DJROOM_CURTAIN_DOOYA_485
#include "config/cl-djroom-curtain-dooya-485.h"
#endif

//长乐DJ会议室射灯
#ifdef DEVICE_CL_DJROOM_LIGHT_MSWITCH2
#include "config/cl-djroom-light-mswitch2.h"
#endif

//长乐VR体验馆党建一楼柯耐弗空调风盘
#ifdef DEVICE_CL_VRHALL_FIRST_FLOOR_AC_OKONOFF
#include "config/cl-vrhall-ff-ac-okonoff.h"
#endif

//长乐VR体验馆党建一楼室外水机McQuay
#ifdef DEVICE_CL_VRHALL_F1_AC_MCQUAY_485
#include "config/cl-vrhall-f1-ac-mcquay-485.h"
#endif


//长乐VR体验馆二三楼Coolix空调
#ifdef DEVICE_CL_VRHALL_F2_AC_COOLIX
#include "config/cl-vrhall-f2-ac-coolix.h"
#endif

//长乐禅修院山水音箱
#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "config/cl-cxy-speaker-sansui-p300.h"
#endif

//长乐小镇美术馆格力空调485
#ifdef DEVICE_CL_XZ_MSG_AC_GREE_485
#include "config/cl-xz-msg-ac-gree-485.h"
#endif

//长乐小镇美术馆格力空调485透传模块
#ifdef DEVICE_CL_XZ_MSG_AC_GREE_PENET
#include "config/cl-xz-msg-ac-gree-penet.h"
#endif

//ESP32_WROOM射频采集器
#ifdef DEVICE_TEST_SNIFFER_ESP32_WROOM
#include "config/test-sniffer-esp32-wroom.h"
#endif

// Z3 协调器 ESP8266 + CC2652 
#ifdef DEVICE_Z3_COORDINATOR_ESP8266_CC2652
#include "config/z3_coordinator_esp8266_cc2652.h"
#endif

#ifndef DEBUGER
#define DEBUGER Serial
#endif



#include <string>
#include <vector>
#include "rfir/util/event-emitter.h"
#include "rfir/util/cjson/CJsonObject.hpp"
class GlobalConfig {
public:
    struct Events
    {
        rfir::util::Event onFixup;
    };

    Events events;    
public:
    GlobalConfig();
    void fixup();
    void fixupTopic();
    void reset();
    void        getIds(neb::CJsonObject* pld, std::string key = "ids");
    std::string expandTopic(std::string topic);
    
public:
    //App
    std::string app_id;
    std::string dom_id;
    std::string dsp_id;
    std::string edg_id;
    std::string dio_id;

    //Device
    std::string dev_id;
    std::string dev_vendor = DEV_VENDOR;
    std::string dev_model = DEV_MODEL;

    //Serial
    int         serial_baud = SERIAL_BAUD;
    int         serial_config = SERIAL_CONFIG;
    //OTA
    bool        ota_update = OTA_UPDATE;
    int         ota_version_number = OTA_VERSION_NUMBER;
    std::string ota_version_string = OTA_VERSION_STRING;
    std::string ota_update_url = OTA_UPDATE_URL;
    //WIFI
    std::vector<std::string>  wifi_ssid_dev = WIFI_SSID_DEV;
    std::vector<std::string>  wifi_password_dev = WIFI_PASSWORD_DEV;
    std::vector<std::string>  wifi_ssid = WIFI_SSID;
    std::vector<std::string>  wifi_password = WIFI_PASSWORD;
    uint32_t                  wifi_reset_timeout = WIFI_RESET_TIMEOUT;

    //AP
    bool                        ap_mode = AP_MODE;
    std::string                 ap_ssid = AP_SSID;
    std::string                 ap_password = AP_PASSWORD;
    int                         ap_reset_timeout = AP_RESET_TIMEOUT;
    std::string                 ap_config_version = AP_CONFIG_VERSION;
    int                         ap_config_pin = AP_CONFIG_PIN;
    int                         ap_config_pin_timeout = AP_CONFIG_PIN_TIMEOUT;
 

    //MQTT
    std::string mqtt_ip = MQTT_IP;
    int         mqtt_port = MQTT_PORT;
    std::string mqtt_user = MQTT_USER;
    std::string mqtt_password = MQTT_PASSWORD;
    int         mqtt_reset_timeout = MQTT_RESET_TIMEOUT;

    //TOPIC

    // //DSP.pub
    // std::string mqtt_dsp_svc_login = MQTT_DSP_SVC_LOGIN;
    //DSP.sub
    std::string mqtt_dsp_evt_status = MQTT_DSP_EVT_STATUS;

    //EDG.pub
    // std::string mqtt_edg_svc_handshake = MQTT_EDG_SVC_HANDSHAKE;
    //EDG.sub
    std::string mqtt_edg_evt_status = MQTT_EDG_EVT_STATUS;


    //DEV.pub
    // std::string mqtt_dev_evt_report = MQTT_DEV_EVT_REPORT;
    // std::string mqtt_dev_evt_status = MQTT_DEV_EVT_STATUS;
    // std::string mqtt_dev_evt_penet = MQTT_DEV_EVT_PENET;

    //DEV.sub
    std::string mqtt_dev_svc_login = MQTT_DEV_SVC_LOGIN;
    std::string mqtt_dev_svc_handshake = MQTT_DEV_SVC_HANDSHAKE;
    std::string mqtt_dev_svc_get = MQTT_DEV_SVC_GET;
    std::string mqtt_dev_svc_set = MQTT_DEV_SVC_SET;
    std::string mqtt_dev_svc_reboot = MQTT_DEV_SVC_REBOOT;
    std::string mqtt_dev_svc_penet = MQTT_DEV_SVC_PENET;



    int         mqtt_resp_timeout = MQTT_RESP_TIMEOUT;
};

extern GlobalConfig Config;
#endif