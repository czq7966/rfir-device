#include "gpio.h"
ICACHE_RAM_ATTR void gpio_interrupt();

std::map<int, rfir::module::ttl::Gpio*>* rfir::module::ttl::Gpio::Gpios = new std::map<int, rfir::module::ttl::Gpio*>();
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_0 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_1 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_2 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_3 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_4 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_5 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_6 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_7 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_8 = 0;
rfir::module::ttl::Gpio* rfir::module::ttl::Gpio::Gpio_9 = 0;

rfir::module::ttl::Gpio::~Gpio() {

}


void rfir::module::ttl::Gpio::init(uint8_t pin, uint8_t mode, bool inverted) {
    this->pin = pin;
    this->mode = mode;
    this->inverted = inverted;
    pinMode(this->pin, this->mode);
}

void rfir::module::ttl::Gpio::uninit() {
    stop();
}

void rfir::module::ttl::Gpio::start() {
    stop();
    //Gpio_0 = this;
    // attachInterrupt(this->pin, gpio_interrupt, CHANGE);  
    //attachInterruptArg(this->pin, Gpio_interruptArg, this, CHANGE);
    OnGpioStart(this);
    _started = true;
}

void rfir::module::ttl::Gpio::stop() {
    //detachInterrupt(this->pin);
    OnGpioStop(this);
    _started = false;
}
int rfir::module::ttl::Gpio::read() {
    if (inverted)
        return !digitalRead(this->pin);
    else 
        return digitalRead(this->pin);
}

void rfir::module::ttl::Gpio::on() {
    if (inverted)
        digitalWrite(pin, LOW);
    else
        digitalWrite(pin, HIGH);
}

void rfir::module::ttl::Gpio::off() {
    if (inverted)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}     

void rfir::module::ttl::Gpio::emitChange() {
    if (onChange) {
        onChange(this, read());
    }
} 

void rfir::module::ttl::Gpio::setArg(void* arg) {this->arg = arg; }
void* rfir::module::ttl::Gpio::getArg() { return this->arg;}
bool rfir::module::ttl::Gpio::started() {return this->_started;}



void rfir::module::ttl::Gpio::OnGpioStart(Gpio* gpio) {
    int idx = -1;
    for (auto it = Gpios->begin(); it != Gpios->end(); it++)
    {
        if (it->second == gpio) {
            idx = it->first;
            break;
        }
    }

    if (idx == -1) {
        for (int i = 0; i < 10; i++)
        {
            auto it = Gpios->find(i);
            if (it == Gpios->end()) {
                Gpios->insert(std::pair<int, Gpio*>(i, gpio));
                idx = i;
                break;
            }
        }
    }

    switch (idx)
    {
        case 0:
            Gpio_0 = gpio;            
            attachInterrupt(gpio->pin, Gpio_0_interrupt, CHANGE);                            
            break;
        case 1:
            Gpio_1 = gpio;
            attachInterrupt(gpio->pin, Gpio_1_interrupt, CHANGE);                               
            break;
        case 2:
            Gpio_2 = gpio;
            attachInterrupt(gpio->pin, Gpio_2_interrupt, CHANGE);                                                           
            break;
        case 3:
            Gpio_3 = gpio;
            attachInterrupt(gpio->pin, Gpio_3_interrupt, CHANGE);                                                           
            break;
        case 4:
            Gpio_4 = gpio;
            attachInterrupt(gpio->pin, Gpio_4_interrupt, CHANGE);                                                           
            break;
        case 5:
            Gpio_5 = gpio;
            attachInterrupt(gpio->pin, Gpio_5_interrupt, CHANGE);                                                           
            break;
        case 6:
            Gpio_6 = gpio;
            attachInterrupt(gpio->pin, Gpio_6_interrupt, CHANGE);                                                           
            break;
        case 7:
            Gpio_7 = gpio;
            attachInterrupt(gpio->pin, Gpio_7_interrupt, CHANGE);                                                           
            break;
        case 8:
            Gpio_8 = gpio;
            attachInterrupt(gpio->pin, Gpio_8_interrupt, CHANGE);                                                           
            break;
        case 9:
            Gpio_9 = gpio;
            attachInterrupt(gpio->pin, Gpio_9_interrupt, CHANGE);                                                           
            break;                                                                                                                                                                                                                                                            
        
        default:
            break;
    }
}
void rfir::module::ttl::Gpio::OnGpioStop(Gpio* gpio) {  

    detachInterrupt(gpio->pin);
    for (auto it = Gpios->begin(); it != Gpios->end(); it++)
    {
        if (it->second == gpio) {            
            Gpios->erase(it);
            break;
        }
    }                    
}               

void rfir::module::ttl::Gpio::Gpio_0_interrupt() {
    if (Gpio_0)
        Gpio_0->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_1_interrupt() {
    if (Gpio_1)
        Gpio_1->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_2_interrupt() {
    if (Gpio_2)
        Gpio_2->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_3_interrupt() {
    if (Gpio_3)
        Gpio_3->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_4_interrupt() {
    if (Gpio_4)
        Gpio_4->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_5_interrupt() {
    if (Gpio_5)
        Gpio_5->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_6_interrupt() {
    if (Gpio_6)
        Gpio_6->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_7_interrupt() {
    if (Gpio_7)
        Gpio_7->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_8_interrupt() {
    if (Gpio_8)
        Gpio_8->emitChange();
}
void rfir::module::ttl::Gpio::Gpio_9_interrupt() {
    if (Gpio_9)
        Gpio_9->emitChange();
}

void rfir::module::ttl::Gpio::Gpio_interruptArg(void* arg) {
    Gpio* gpio = (Gpio*)arg;
    if (gpio)
        gpio->emitChange();
}
