#ifndef SERVICE_DEVICE_H
#define SERVICE_DEVICE_H

#include "config-device.h"
#include "device/device.h"

//设备基类
#ifdef DEVICE_BASE
#include "config/base.h"
#endif

//串口透传
#ifdef DEVICE_COM_PENET
#include "config/com-penet.h"
#endif


//串口透传
#ifdef DEVICE_COM_PENET_Z3
#include "config/com-penet-z3.h"
#endif

extern device::Device* GDevice;  

#endif                
