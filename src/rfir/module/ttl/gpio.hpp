//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_TTL_GPIO_H__
#define __RFIR_MODULE_TTL_GPIO_H__

#include "rfir/util/platform.h"
#include <list>
#include <mutex>

namespace rfir {
    namespace module {
        namespace ttl {
            class Gpio {         
            private:       
                uint8_t outputOn;
                uint8_t outputOff;
            public:              
                uint8_t pin = 0;
                uint8_t mode = 0;
                bool    inverted = false;
                std::function<void(int)> callback = 0;

                
                ~Gpio() {
                    this->uninit();
                }

                void init(uint8_t pin, uint8_t mode = OUTPUT, bool inverted = false) {
                    this->pin = pin;
                    this->mode = mode;
                    this->inverted = inverted;
                    pinMode(this->pin, this->mode);
                }

                void uninit() {
                    detachInterrupt(this->pin);
                }

                int read() {
                    if (inverted)
                        return !digitalRead(this->pin);
                    else 
                        return digitalRead(this->pin);
                }

                void on() {
                    if (inverted)
                        digitalWrite(pin, LOW);
                    else
                        digitalWrite(pin, HIGH);
                }

                void off() {
                    if (inverted)
                        digitalWrite(pin, HIGH);
                    else
                        digitalWrite(pin, LOW);
                }     

                void onChange(std::function<void(int)> cb = 0) {

                }          

                void emitChange() {
                    if (callback)
                        callback(read());
                } 
            };
        }
    }
}


#endif //__RFIR_MODULE_SERIAL_RECEIVER_H__
