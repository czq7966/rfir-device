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
        rfir::util::Util::StringToBytes("0x" + code, bytes);
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
    std::string name, code;
    neb::CJsonObject entry;
    neb::CJsonObject* hd = &cmd->command.hd;

    if (pld->Get("code", code)) {
        return RS::RS485::onSvc_set(pld, cmd); 
    }

    if (hd->Get("entry", entry) && entry.Get("name", name)) {
        if (name == "open")
            return onSvc_set_open(pld, cmd);
        if (name == "close")
            return onSvc_set_close(pld, cmd);
        if (name == "stop")
            return onSvc_set_stop(pld, cmd);
        if (name == "pos")
            return onSvc_set_pos(pld, cmd);
        if (name == "dir")
            return onSvc_set_dir(pld, cmd);
        if (name == "up_travel")
            return onSvc_set_up_travel(pld, cmd);
        if (name == "down_travel")
            return onSvc_set_down_travel(pld, cmd);
        if (name == "del_travel")
            return onSvc_set_del_travel(pld, cmd);            
        if (name == "power_on_prompt")
            return onSvc_set_power_on_prompt(pld, cmd); 
    } else {
        int pos = -1;
        int dir = 0;        
        if (pld->Get("dir", dir)) 
            result = onSvc_set_dir(pld, cmd);
        if (pld->Get("pos", pos)) 
            result = onSvc_set_pos(pld, cmd);        
    }

    return result;

}

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_open(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string code = "0x55 FE FE 03 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    DEBUGER.println(code.c_str());
    return RS::RS485::onSvc_set(pld, cmd);        
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_close(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 02";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    return RS::RS485::onSvc_set(pld, cmd);  
}; 
bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_stop(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 03";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_pos(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {    
    std::string code;
    int pos = 0;
    if (pld->Get("pos", pos) && (pos >=0 && pos <=100)) {        
        code = "0x55 FE FE 03 04" + rfir::util::Util::BytesToHexString((uint8_t*)&pos, 1);        
        code = rfir::util::Crc::Get_CRC16_Str(code);
        pld->ReplaceAdd("code", code);
        return RS::RS485::onSvc_set(pld, cmd);    
    } 

    return 0;
};

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_dir(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {    
    std::string code;
    int dir = 0;
    if (pld->Get("dir", dir)) {
        if (dir == 0)
            code = "0x55 FE FE 02 03 01 00";
        if (dir == 1)
            code = "0x55 FE FE 02 03 01 01";
    } 

    if (code != "") {
        code = rfir::util::Crc::Get_CRC16_Str(code);
        pld->ReplaceAdd("code", code);
        return RS::RS485::onSvc_set(pld, cmd);     
    }

    return 0;
}; 
bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_up_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 05 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_down_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 05 02";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_del_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 07 FF";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    pld->ReplaceAdd("code", code);
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_power_on_prompt(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code;
    int prompt = 0;
    if (pld->Get("prompt", prompt)) {
        if (prompt == 0)
            code = "0x55 FE FE 02 0C 01 00";
        if (prompt == 1)
            code = "0x55 FE FE 02 0C 01 01";
        if (prompt == 2)
            code = "0x55 FE FE 02 0C 01 02";
        if (prompt == 3)
            code = "0x55 FE FE 02 0C 01 03";            
    } 

    if (code != "") {
        code = rfir::util::Crc::Get_CRC16_Str(code);
        pld->ReplaceAdd("code", code);
        return RS::RS485::onSvc_set(pld, cmd);     
    }

    return 0; 
}; 
