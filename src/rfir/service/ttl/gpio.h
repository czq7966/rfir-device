//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_GPIO_GPIO_H__
#define __RFIR_SERVICE_GPIO_GPIO_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/gpio.hpp"
#include <list>
#include <mutex>



namespace rfir {
    namespace service {
        namespace ttl {
            class Gpio {
            public:
                static rfir::module::ttl::Gpio* gpio;
                static void init(uint8_t pin, uint8_t mode, bool inverted = false);
                static void start();
                static void stop();

                static void gpio_interrupt();            
            };
        }
    }
}


#endif //__RFIR_SERVICE_GPIO_GPIO_H__
