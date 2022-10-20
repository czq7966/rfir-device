#include "device.h"

//8285红外接收发射透传
#ifdef DEVICE_BASE
#include "device/device.h"
device::Device* GDevice = new device::Device();
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

