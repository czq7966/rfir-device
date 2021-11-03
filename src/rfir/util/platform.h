//
// Created by Administrator on 2020/12/15.
//

#ifndef __RFIR_UTIL_PLATFORM_H__
#define __RFIR_UTIL_PLATFORM_H__


#ifdef ESP32
#include <Arduino.h>
#include <iostream>
#include <string.h>
#include <sstream>   

#include <utility>
#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <mutex>
#include <sys/time.h>

#include "FS.h"
#include <LITTLEFS.h>
#include "cjson/CJsonObject.hpp"

#define FileFS LITTLEFS


#endif //ESP32

#ifdef ESP8266

#include <Arduino.h>
#include <iostream>
#include <string>
#include <sstream>   

#include <utility>
#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <mutex>
#include <sys/time.h>
#include <FS.h>
#include <LittleFS.h>
#include "cjson/CJsonObject.hpp"

#define FileFS LittleFS

#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timerclear(tvp)		((tvp)->tv_sec = (tvp)->tv_usec = 0)
#define	timercmp(a, b, CMP) 						      \
  (((a)->tv_sec == (b)->tv_sec) ? 					      \
   ((a)->tv_usec CMP (b)->tv_usec) : 					      \
   ((a)->tv_sec CMP (b)->tv_sec))
#define	timeradd(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;			      \
    if ((result)->tv_usec >= 1000000)					      \
      {									      \
	++(result)->tv_sec;						      \
	(result)->tv_usec -= 1000000;					      \
      }									      \
  } while (0)
#define	timersub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_usec += 1000000;					      \
    }									      \
  } while (0)



#endif //ESP8266



#endif
