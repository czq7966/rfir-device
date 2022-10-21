#ifndef CONFIG_COM_PENET_HALF_H
#define CONFIG_COM_PENET_HALF_H

#include "com-penet.h"

#undef  DEV_MODEL
#define DEV_MODEL                   "COM-PENET-HALF"

//Serial
#undef  SERIAL_HALF_RW_PIN
#undef  SERIAL_READ_TIMEOUT
#define SERIAL_READ_TIMEOUT         20
#define SERIAL_HALF_RW_PIN          5


#undef  OTA_VERSION
#undef  OTA_UPDATE_URL
#define OTA_VERSION                 2
#define OTA_UPDATE_URL              "http://betacs.101.com/v0.1/static/preproduction_content_ndcast_ota/ota/v3/com-penet-half/cfg.txt"

#endif