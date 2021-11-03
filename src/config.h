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
#ifdef DEVICE_FZ_DM4_AC_GREE
#include "config/config-fz-dm4-ac-gree.h"
#endif

//长乐DJ会议室Mcquay空调
#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "config/config-cl-djroom-ac-mcquay.h"
#endif

#endif