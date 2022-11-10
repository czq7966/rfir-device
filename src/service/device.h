#ifndef SERVICE_DEVICE_H
#define SERVICE_DEVICE_H

#include "config-device.h"

//设备基类
#ifdef DEVICE_BASE
#include "config/base.h"
#endif

//射频透传
#ifdef DEVICE_RFIR_PENET
#include "config/rfir-penet.h"
#endif

//射频透传
#ifdef DEVICE_RFIR_PENET_8285
#include "config/rfir-penet-8285.h"
#endif

//射频透传
#ifdef DEVICE_RFIR_PENET_ND_AC
#include "config/rfir-penet-nd-ac.h"
#endif

//串口透传
#ifdef DEVICE_COM_PENET
#include "config/com-penet.h"
#endif


//Z3串口透传
#ifdef DEVICE_COM_PENET_Z3
#include "config/com-penet-z3.h"
#endif

//串口半双工透传
#ifdef DEVICE_COM_PENET_HALF
#include "config/com-penet-half.h"
#endif

#include "device/device.h"
extern device::Device* GDevice;  

#endif                
