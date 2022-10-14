#include "serial.h"

rfir::util::SerialConfigs::SerialConfigs(){
    add(SERIAL_8N1, "8N1");
    add(SERIAL_8N2, "8N2");
    add(SERIAL_8O1, "8O1");
    add(SERIAL_8O2, "8O2");
    add(SERIAL_8E1, "8E1");
    add(SERIAL_8E2, "8E2");
};

void rfir::util::SerialConfigs::add(int key, const char* value){
    auto name = new char[strlen(value)];
    strcpy(name, value);
    configs.add(key, name);
};

int rfir::util::SerialConfigs::getSerialBaud(){
    return this->baud;
};
int rfir::util::SerialConfigs::getSerialConfig(){
    int ndata = (this->data >= 5 && this->data <=8) ? this->data : 8;
    ndata = ((ndata - 5) << 2) & UART_NB_BIT_MASK;
    int nstop = this->stop == 0 ? UART_NB_STOP_BIT_0 :  this->stop == 2 ? UART_NB_STOP_BIT_2 : UART_NB_STOP_BIT_1;
    int nsum = this->sum == 1 ? UART_PARITY_ODD :  this->sum == 2 ? UART_PARITY_EVEN : UART_PARITY_NONE;

    return ndata | nstop | nsum;    
};

int rfir::util::SerialConfigs::getSerialData(){
    return this->data;
};
int rfir::util::SerialConfigs::getSerialStop(){
    return this->stop;
};
int rfir::util::SerialConfigs::getSerialSum(){
    return this->sum;
};

void rfir::util::SerialConfigs::setSerialBaud(int pbaud){
    this->baud = pbaud;
};

void rfir::util::SerialConfigs::setSerialConfig(int pconfig){
    int ndata = ((pconfig & UART_NB_BIT_MASK) >> 2) + 5;
    int nstop = pconfig & UART_NB_STOP_BIT_MASK;
    nstop = nstop == UART_NB_STOP_BIT_0 ? 0 : nstop == UART_NB_STOP_BIT_2 ? 2: 1;
    int nsum = pconfig & UART_PARITY_MASK;
    nsum = nsum == UART_PARITY_ODD ? 1 : nsum == UART_PARITY_EVEN ? 2 : 0;

    setSerialData(ndata);
    setSerialStop(nstop);
    setSerialSum(nsum);
};

void rfir::util::SerialConfigs::setSerialConfig(const char* pconfig){
    auto map = configs.getMap();
    for (auto it = map->begin(); it != map->end(); it++)
    {
        if (strcmp(it->second, pconfig) == 0) {
            setSerialConfig(it->first);
            break;
        }
    }
};

void rfir::util::SerialConfigs::setSerialData(int pdata){
    this->data = pdata;
};
void rfir::util::SerialConfigs::setSerialStop(int pstop){
    this->stop = pstop;
};
void rfir::util::SerialConfigs::setSerialSum(int psum){
    this->sum = psum;
};

rfir::util::SerialConfigs GSerialConfigs;
