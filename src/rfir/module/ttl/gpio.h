//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_TTL_GPIO_H__
#define __RFIR_MODULE_TTL_GPIO_H__

#include "rfir/util/platform.h"
#include <list>
#include <mutex>

void gpio_interrupt();

namespace rfir {
    namespace module {
        namespace ttl {
            class Gpio {         
            private:       
                uint8_t outputOn;
                uint8_t outputOff;
                void*   arg = 0;
                bool    _started = false;
            public:              
                uint8_t pin = 0;
                uint8_t mode = 0;
                bool    inverted = false;
                
                typedef std::function<void(Gpio* gpio, int value)> OnChange;  
                
                OnChange onChange = 0;

                ~Gpio();

                void init(uint8_t pin, uint8_t mode = OUTPUT, bool inverted = false);
                void uninit();
                void start();
                void stop();
                int read();
                void on();
                void off();
                void emitChange();
                void setArg(void* arg);
                void* getArg();
                bool started();

            public:
                static std::map<int, Gpio*>* Gpios;
                static Gpio* Gpio_0;
                static Gpio* Gpio_1;
                static Gpio* Gpio_2;
                static Gpio* Gpio_3;
                static Gpio* Gpio_4;
                static Gpio* Gpio_5;
                static Gpio* Gpio_6;
                static Gpio* Gpio_7;
                static Gpio* Gpio_8;
                static Gpio* Gpio_9;

                static void OnGpioStart(Gpio* gpio);
                static void OnGpioStop(Gpio* gpio);
                static ICACHE_RAM_ATTR void Gpio_0_interrupt();
                static ICACHE_RAM_ATTR void Gpio_1_interrupt();
                static ICACHE_RAM_ATTR void Gpio_2_interrupt();
                static ICACHE_RAM_ATTR void Gpio_3_interrupt();
                static ICACHE_RAM_ATTR void Gpio_4_interrupt();
                static ICACHE_RAM_ATTR void Gpio_5_interrupt();
                static ICACHE_RAM_ATTR void Gpio_6_interrupt();
                static ICACHE_RAM_ATTR void Gpio_7_interrupt();
                static ICACHE_RAM_ATTR void Gpio_8_interrupt();
                static ICACHE_RAM_ATTR void Gpio_9_interrupt();
                static ICACHE_RAM_ATTR void Gpio_interruptArg(void* arg);
            };
        }
    }
}


#endif //__RFIR_MODULE_SERIAL_RECEIVER_H__
