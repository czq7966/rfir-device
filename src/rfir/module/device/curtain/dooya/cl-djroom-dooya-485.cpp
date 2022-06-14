#include "cl-djroom-dooya-485.h"
#include "rfir/util/crc.h"


void rfir::module::device::curtain::CL_DJROOM_Dooya_485::init() {
    RS485::init();
    this->name = "dooya";
}


bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string innerCode = "0x55 FE FE 01 00 10 44 2E";
    std::string code;
    if (!pld->Get("code", code) || code.length() == 0) {
        pld->ReplaceAdd("code", innerCode);
    }

    if (RS485::onSvc_get(pld, cmd) && pld->Get("code", code)) {
        uint8_t bytes[code.length()];
        rfir::util::Util::StringToBytes(code, bytes);
        if (bytes[3] == 0x01 && bytes[4] == 0x10) {
            pld->ReplaceAdd("pos", bytes[7]);
            pld->ReplaceAdd("dir", bytes[8]);
            pld->ReplaceAdd("state", bytes[10]);
            pld->ReplaceAdd("angel", bytes[11]);
            pld->ReplaceAdd("angelDir", bytes[12]);
            pld->ReplaceAdd("angelFac", bytes[13]);
            pld->ReplaceAdd("travelState", bytes[14]);
            pld->ReplaceAdd("jogMode", bytes[15]);
            pld->ReplaceAdd("powerOnPrompt", bytes[17]);
        }
    }
    return 0;
}

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = false;
    std::string head = "0x55 FE FE";
    std::string code;

    if (pld->Get("code", code)) {
        return RS::RS485::onSvc_set(pld, cmd);
    };

    int mode = 0;
    int pos = -1;
    int dir = 0;
    int travelState = 0;
    int powerOnPrompt = 0;

    pld->Get("mode", mode);

    if (mode == 0 && pld->Get("pos", pos)) {
        if (pos == 100)
            code = head + " 03 01";
        if (pos == 0)
            code = head + " 03 02";
        if (pos == -1)
            code = head + " 03 03";
        if (pos > 0 && pos < 100) 
            code = code + " 04" + rfir::util::Util::BytesToHexString((uint8_t*)&pos, 1);
        
        if (code != "") {
            code = rfir::util::Crc::Get_CRC16_Str(code);
            pld->ReplaceAdd("code", code);
            result = RS::RS485::onSvc_set(pld, cmd);        
        }   
    }     
    
    if (mode == 1 && pld->Get("dir", dir)) {
        if (dir == 0)
            code = head + " 02 03 01 00";
        if (dir == 1)
            code = head + " 02 03 01 01";

        if (code != "") {
            code = rfir::util::Crc::Get_CRC16_Str(code);
            pld->ReplaceAdd("code", code);
            result = RS::RS485::onSvc_set(pld, cmd);        
        }
    } 
    
    if (mode == 1 && pld->Get("powerOnPrompt", powerOnPrompt)) {
        if (travelState == 0)
            code = head + " 02 0C 01 00";
        if (travelState == 1)
            code = head + " 02 0C 01 01";
        if (travelState == 2)
            code = head + " 02 0C 01 02";
        if (travelState == 3)
            code = head + " 02 0C 01 03";  

        if (code != "") {
            code = rfir::util::Crc::Get_CRC16_Str(code);
            pld->ReplaceAdd("code", code);
            result = RS::RS485::onSvc_set(pld, cmd);        
        }   
    } 
    
    if (mode == 1 && pld->Get("travelState", travelState)) {
        if (travelState == 1)
            code = head + " 03 05 01";
        if (travelState == 2)
            code = head + " 03 05 02";
        if (travelState == 255)
            code = head + " 03 07 FF";

        if (code != "") {
            code = rfir::util::Crc::Get_CRC16_Str(code);
            pld->ReplaceAdd("code", code);
            result = RS::RS485::onSvc_set(pld, cmd);        
        }               
    }

    return result;
}