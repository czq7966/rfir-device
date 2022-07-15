#include "rs485-modbus.h"
#include "rfir/util/crc.h"


rfir::module::device::RS::RS485_MODBUS::Cmd_Read_Coils::Cmd_Read_Coils() {
    func = 0x01;
}

void rfir::module::device::RS::RS485_MODBUS::Cmd_Read_Coils::fixUp() {
    
}


void rfir::module::device::RS::RS485_MODBUS::init() {
    this->recvTimeoutMS = 200;
    this->name = "RS485_MODBUS";
    RS485::init();
}

void rfir::module::device::RS::RS485_MODBUS::loop() {

}

bool rfir::module::device::RS::RS485_MODBUS::checkSumCode(uint8_t* rx_buf, uint8_t len) {
    bool result = false;
    if (len > 2) {
        uint8_t crcLow;
        uint8_t crcHigh;
        rfir::util::Crc::Get_CRC16(rx_buf, len - 2, crcLow, crcHigh);
        result = (rx_buf[len - 1] == crcHigh && rx_buf[len - 2] == crcLow);
    }  

    return result;
}; 


int rfir::module::device::RS::RS485_MODBUS::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = true;
    std::string key;
    neb::CJsonObject jError;
    int modAddr = m_modAddr;
    pld->Get("mod_address", modAddr);
    rfir::util::MapFactory<std::string, uint16_t> keys;
    while (pld->GetKey(key)) {
        keys.add(key, 0);
    }

    if (keys.getSize() == 0) {
        for (auto it = m_regAddrs.getMap()->begin(); it != m_regAddrs.getMap()->end(); it++) {
             keys.add(it->first, 0);
        }
    }

    for (auto it = keys.getMap()->begin(); it != keys.getMap()->end(); it++)
    {
        uint16_t regAddr;
        if (m_regAddrs.get(it->first, regAddr)) {
            uint32_t value = 0;
            uint32_t error = 0;

            bool res = getRegOne(modAddr, regAddr, value, error);
            if (res) {
                pld->ReplaceAdd(it->first, value);
            } else {
                jError.ReplaceAdd(it->first, error);
            };

            result = result && res;
        };        
    }

    if (!result) {
        pld->ReplaceAdd("_error", jError);
    }

    return result;    
}

int rfir::module::device::RS::RS485_MODBUS::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = true;
    std::string key;
    neb::CJsonObject jError;
    int modAddr = m_modAddr;    
    pld->Get("mod_address", modAddr);
    rfir::util::MapFactory<std::string, uint16_t> keys;
    while (pld->GetKey(key)) {
        keys.add(key, 0);
    }

    for (auto it = keys.getMap()->begin(); it != keys.getMap()->end(); it++)
    {
        uint16_t regAddr;
        if (m_regAddrs.get(it->first, regAddr)) {
            int value = 0;
            uint32_t error = 0;
            if (pld->Get(it->first, value)) {
                bool res = setRegOne(modAddr, regAddr, value, error);
                if (res) {
                    pld->ReplaceAdd(it->first, value);
                } else {
                    jError.ReplaceAdd(it->first, error);                
                };
                result = result && res;
            }            
        };        
    }

    if (!result) {
        pld->ReplaceAdd("_error", jError);
    }

    return result;       

}   


bool rfir::module::device::RS::RS485_MODBUS::splitRegAddr(uint32_t regAddr, int8_t& func, uint16_t& addr, int writeOffset) {
    uint32_t type = regAddr / 10000;
    addr = regAddr % 10000;
    func = -1;

    if (writeOffset >=0) {
        if (type == 0) func = 5 + writeOffset;
        else if (type == 4) func = 6 + writeOffset;
    } else {
        if (type == 0) func = 1;
        else if (type == 1) func = 2;
        else if (type == 3) func = 4;
        else if (type == 4) func = 3;
    }

    return func != -1;
}

bool rfir::module::device::RS::RS485_MODBUS::getRegOne(uint8_t modAddr, uint32_t regAddr, uint32_t& value, uint32_t& error) {
    int8_t      funcCode = 0;
    uint16_t    addr = 0;
    if (splitRegAddr(regAddr, funcCode, addr)) {
        txBuf[0] = modAddr;
        txBuf[1] = funcCode;
        txBuf[2] = ((uint8_t*)&addr)[1];
        txBuf[3] = ((uint8_t*)&addr)[0];
        txBuf[4] = 0;
        txBuf[5] = 1;

        rfir::util::Crc::Get_CRC16(txBuf, 6, txBuf[6], txBuf[7]);

        uint8_t len = 0;
        if(sendCodeAndRecv(txBuf, 8, rxBuf, len, this->recvTimeoutMS)) {
            if (rxBuf[0] == modAddr) {
                if (rxBuf[1] == funcCode && rxBuf[2] == txBuf[5] * 2) {
                    if (rxBuf[2] == txBuf[5] )
                        value = rxBuf[3];
                    if (rxBuf[2] == txBuf[5] * 2)
                        value = (rxBuf[3] << 8) + rxBuf[4];
                    return true;
                } else if (rxBuf[1] == (0x80 + funcCode)) {
                    error = ((0x80 + funcCode) << 16) + rxBuf[2];
                }
            }
        }
    }

    return false;
}

bool rfir::module::device::RS::RS485_MODBUS::setRegOne(uint8_t modAddr, uint32_t regAddr, uint32_t value, uint32_t& error) {
    int8_t      funcCode = 0;
    uint16_t    addr = 0;
    if (splitRegAddr(regAddr, funcCode, addr, 0)) {
        txBuf[0] = modAddr;
        txBuf[1] = funcCode;
        txBuf[2] = ((uint8_t*)&addr)[1];
        txBuf[3] = ((uint8_t*)&addr)[0];
        txBuf[4] = ((uint8_t*)&value)[1];
        txBuf[5] = ((uint8_t*)&value)[0];
        if (funcCode == 5) {
            txBuf[4] = value == 0 ? 0 : 0xff;
            txBuf[5] = 0;
        }


        rfir::util::Crc::Get_CRC16(txBuf, 6, txBuf[6], txBuf[7]);

        uint8_t len = 0;
        if(sendCodeAndRecv(txBuf, 8, rxBuf, len, this->recvTimeoutMS)) {
            if (rxBuf[0] == modAddr) {
                if (rxBuf[1] == funcCode) {
                    return true;
                } else if (rxBuf[1] == (0x80 + funcCode)) {
                    error = ((0x80 + funcCode) << 16) + rxBuf[2];
                }
            }
        }
    }

    return false;
}
