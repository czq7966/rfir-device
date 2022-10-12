#include "serial.h"

rfir::util::Serial_Configs::Serial_Configs(){
    configs.add(SERIAL_8N1, "8N1");
    configs.add(SERIAL_8N2, "8N2");
    configs.add(SERIAL_8O1, "8O1");
    configs.add(SERIAL_8O2, "8O2");
    configs.add(SERIAL_8E1, "8E1");
    configs.add(SERIAL_8E2, "8E2");
};

rfir::util::Serial_Configs GSerial_Configs;
