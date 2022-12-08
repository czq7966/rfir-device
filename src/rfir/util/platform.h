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
#include <vector>
#include <mutex>
#include <sys/time.h>

#include "FS.h"
#include <LITTLEFS.h>
#include <WiFi.h>

#define FileFS LittleFS 
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define SerialConfig uint32_t

// Options for `config` argument of uart_init
#define UART_CONFIG_MASK      0B10000000

#define UART_NB_BIT_MASK      0B00001100
#define UART_NB_BIT_5         0B00000000
#define UART_NB_BIT_6         0B00000100
#define UART_NB_BIT_7         0B00001000
#define UART_NB_BIT_8         0B00001100

#define UART_PARITY_MASK      0B00000011
#define UART_PARITY_NONE      0B00000000
#define UART_PARITY_EVEN      0B00000010
#define UART_PARITY_ODD       0B00000011

#define UART_NB_STOP_BIT_MASK 0B00110000
#define UART_NB_STOP_BIT_0    0B00000000
#define UART_NB_STOP_BIT_1    0B00010000
#define UART_NB_STOP_BIT_15   0B00100000
#define UART_NB_STOP_BIT_2    0B00110000


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
#include <ESP8266WiFi.h>

#define FileFS LittleFS

// Options for `config` argument of uart_init
#define UART_CONFIG_MASK      0B00000000

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
