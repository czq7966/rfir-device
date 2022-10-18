#include "device.h"

//8285红外接收发射透传
#ifdef DEVICE_BASE
#include "device/device.h"
device::Device* GDevice = new device::Device();
#endif

#ifdef DEVICE_COM_PENET
#include "device/com-device.h"
device::Device* GDevice = new device::ComDevice();
#endif