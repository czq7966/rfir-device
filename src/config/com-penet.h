#ifndef CONFIG_COM_PENET_H
#define CONFIG_COM_PENET_H

#include "base.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "COM-PENET"

//Serial
#undef  SERIAL_READ_TIMEOUT
#define SERIAL_READ_TIMEOUT         10

#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 1
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/com-penet/cfg.txt"

#endif