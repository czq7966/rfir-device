#include "cl-xz-msg-gree-485.h"
#include "rfir/util/crc.h"


void rfir::module::device::ac::CL_XZ_MSG_GREE_485::init() {
    rfir::module::device::RS::RS485_MODBUS::init();
    this->name = "CL_XZ_MSG_GREE_485";
}

std::string rfir::module::device::ac::CL_XZ_MSG_GREE_485::getAddrHex() {
    return rfir::util::Util::BytesToHexString(&m_addr, 1);
}

bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = 0;
    std::string code;
    if (!pld->Get("code", code) && code.length() != 0) {
        return RS485::onSvc_get(pld, cmd);
    }

    result = onSvc_get_power(pld, cmd) && onSvc_get_mode(pld, cmd) && onSvc_get_temp(pld, cmd); 

    return result;
}


bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = false;
    result = onSvc_set_power(pld, cmd) || result;
    result = onSvc_set_mode(pld, cmd) || result;
    result = onSvc_set_temp(pld, cmd) || result;

    return result;

}


bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_get_power(neb::CJsonObject* p_pld, cmds::cmd::CmdBase* cmd) {
    //Power
    bool result = 0;
    std::string code;
    neb::CJsonObject pld;
    code = "0x" + getAddrHex() + " 01 00 01 00 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld.ReplaceAdd("code", code);
    if (RS485::onSvc_get(&pld, cmd) && pld.Get("code", code)) {
        uint8_t bytes[code.length()];
        rfir::util::Util::StringToBytes("0x" + code, bytes);
        if (bytes[0] == m_addr && bytes[1] == 0x01 && bytes[2] == 0x01) {
            int power = bytes[0 + 3];
            p_pld->ReplaceAdd("power", power == 1 ? "on" : "off");
            result = 1;
        }        
    }
    return result;
}; 

bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_get_mode(neb::CJsonObject* p_pld, cmds::cmd::CmdBase* cmd) {
    //Mode
    bool result = 0;
    std::string code;
    neb::CJsonObject pld;
    code = "0x" + getAddrHex() + " 03 13 8A 00 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld.ReplaceAdd("code", code);
    if (RS485::onSvc_get(&pld, cmd) && pld.Get("code", code)) {
        uint8_t bytes[code.length()];
        rfir::util::Util::StringToBytes("0x" + code, bytes);
        if (bytes[0] == m_addr && bytes[1] == 0x03 && bytes[2] == 0x0001 * 2) {
            int mode = bytes[1 + 3];
            p_pld->ReplaceAdd("mode", mode == 1 ? "cool" : mode == 2 ? "head": "fan");
            result = 1;
        }        
    }
    return result;
}; 
 
bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_get_temp(neb::CJsonObject* p_pld, cmds::cmd::CmdBase* cmd) {
    //Mode
    bool result = 0;
    std::string code;
    neb::CJsonObject pld;
    code = "0x" + getAddrHex() + " 03 00 09 00 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld.ReplaceAdd("code", code);
    if (RS485::onSvc_get(&pld, cmd) && pld.Get("code", code)) {
        uint8_t bytes[code.length()];
        rfir::util::Util::StringToBytes("0x" + code, bytes);        
        if (bytes[0] == m_addr && bytes[1] == 0x03 && bytes[2] == 0x0001 * 2) {
            int temp = bytes[0 + 3] << 8 + bytes[1 + 3];
            p_pld->ReplaceAdd("temperature", temp / 10);
            result = 1;
        }        
    }
    return result;
}; 
 
bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_set_power(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = false;
    std::string code;

    //Power
    std::string power;   
    if (pld->Get("power", power)) {
        code = "0x" +  getAddrHex() + " 05 00 02";
        power = rfir::util::Util::ToLower(power.c_str());
        if (power == "on" || power == "off") {
            code += power == "on" ? " ff 00" : " 00 00";
            code = rfir::util::Crc::Get_CRC16_Str(code);
            result = RS::RS485::onSvc_set(pld, cmd); 
        }
    }


    return result;
}; 
 
// bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_set_mode(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
//     bool result = false;
//     std::string code;

//     //Mode
//     std::string mode;   
//     if (pld->Get("mode", mode)) {
//         code = "0x" + m_addr + " 06 13 8B";
//         mode = rfir::util::Util::ToLower(mode.c_str());
//         if (mode == "fan" || mode == "cool" || mode == "heat") {
//             code += (mode == "cool" ? " 00 01" : (mode == "heat" ? " 00 02" : " 00 00"));
//             code = rfir::util::Crc::Get_CRC16_Str(code);
//             result = RS::RS485::onSvc_set(pld, cmd); 
//         }
//     }


//     return result;
// }; 
 
// bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_set_temp(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
//     bool result = false;
//     std::string code;

//     //Mode
//     uint16_t temp;   
//     int temperature;
//     if (pld->Get("temperature", temperature)) {
//         temp = temperature;
//         code = "0x" + m_addr + " 06 00 0A";
//         temp = temp * 10;
//         code += rfir::util::Util::BytesToHexString((uint8_t*)&temp, 2, true);
//         code = rfir::util::Crc::Get_CRC16_Str(code);
//         result = RS::RS485::onSvc_set(pld, cmd);         
//     }

//     return result;
// }; 
                     
