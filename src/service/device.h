#ifndef SERVICE_DEVICE_H
#define SERVICE_DEVICE_H

#include "config-device.h"
#include "device/device.h"

//设备基类配置
#ifdef DEVICE_BASE
#include "config/base.h"
#endif

extern device::Device* GDevice;  

#endif                
