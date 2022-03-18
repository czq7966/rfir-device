#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "config-device.h"

#define DEGUG true

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

//福州东门3楼Media空调
#ifdef DEVICE_FZ_DM3_AC_MEDIA
#include "config/fz-dm3-ac-media.h"
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
#ifdef DEVICE_CL_DJROOM_CURTAIN_DOOYA
#include "config/cl-djroom-curtain-dooya.h"
#endif

//长乐DJ会议室射灯
#ifdef DEVICE_CL_DJROOM_LIGHT_MSWITCH2
#include "config/cl-djroom-light-mswitch2.h"
#endif

//长乐VR体验馆党建一楼柯耐弗空调风盘
#ifdef DEVICE_CL_VRHALL_FIRST_FLOOR_AC_OKONOFF
#include "config/cl-vrhall-ff-ac-okonoff.h"
#endif

//长乐VR体验馆二三楼Coolix空调
#ifdef DEVICE_CL_VRHALL_F2_AC_COOLIX
#include "config/cl-vrhall-f2-ac-coolix.h"
#endif

//长乐禅修院山水音箱
#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "config/cl-cxy-speaker-sansui-p300.h"
#endif


//ESP32_WROOM射频采集器
#ifdef DEVICE_TEST_SNIFFER_ESP32_WROOM
#include "config/test-sniffer-esp32-wroom.h"
#endif

#endif