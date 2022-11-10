#include "device.h"

//设备基类
#ifdef DEVICE_BASE
#include "device/device.h"
device::Device* GDevice = new device::Device();
#endif

//射频透传: 8285,ND定制空调
#if defined(DEVICE_RFIR_PENET) || defined(DEVICE_RFIR_PENET_8285) || defined(DEVICE_RFIR_PENET_ND_AC)
#include "device/rfir-device.h"
device::Device* GDevice = new device::RfirDevice();
#endif

//串口透传
#ifdef DEVICE_COM_PENET
#include "device/com-device.h"
device::Device* GDevice = new device::ComDevice();
#endif

//串口透传Z3
#ifdef DEVICE_COM_PENET_Z3
#include "device/z3/com-device-z3.h"
device::Device* GDevice = new device::ComDeviceZ3();
#endif


//串口半双工透传
#ifdef DEVICE_COM_PENET_HALF
#include "device/com-device-half.h"
device::Device* GDevice = new device::ComDeviceHalf();
#endif