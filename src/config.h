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

//长乐DJ会议室Mcquay空调
#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "config/cl-djroom-ac-mcquay.h"
#endif

//长乐禅修院山水音箱
#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "config/cl-cxy-speaker-sansui-p300.h"
#endif

#endif