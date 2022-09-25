#include "cl-djroom-dooya-485.h"
#include "rfir/util/crc.h"
#include "rfir/util/util.h"


void rfir::module::device::curtain::CL_DJROOM_Dooya_485::init() {
    RS485::init();
    this->name = "dooya";
}


int rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string innerCode = "0x55 FE FE 01 00 10 44 2E";
    std::string code;
    if (pld->containsKey("code")) 
        code = (*pld)["code"].as<std::string>();

    if (code.length() == 0) 
        code = innerCode;

    (*pld)["code"] = code;       
    

    if (RS485::onSvc_get(pld, cmd) && pld->containsKey("code")) {
        code = (*pld)["code"].as<std::string>();
        uint8_t bytes[code.length()];
        rfir::util::Util::StringToBytes("0x" + code, bytes);
        if (bytes[3] == 0x01 && bytes[4] == 0x10) {
            (*pld)["pos"] = bytes[7];
            (*pld)["dir"] = bytes[8];
            (*pld)["state"] = bytes[10];
            (*pld)["angel"] = bytes[11];
            (*pld)["angelDir"] = bytes[12];
            (*pld)["angelFac"] = bytes[13];
            (*pld)["travelState"] = bytes[14];
            (*pld)["jogMode"] = bytes[15];
            (*pld)["powerOnPrompt"] = bytes[17];
            return 1;
        }        
    }
    return 0;
}

int rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    bool result = false;
    // std::string name, code;
    
    JsonObject hd = cmd->command.hd;
    JsonObject entry = hd["entry"];

    if (pld->containsKey("code")) 
        return RS::RS485::onSvc_set(pld, cmd);     

    if (!entry.isNull() && entry.containsKey("name")) {
        std::string name = entry["name"];        
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
        if (pld->containsKey("dir")) 
            result = onSvc_set_dir(pld, cmd);
        
        if (pld->containsKey("pos"))
            result = onSvc_set_pos(pld, cmd);        
    }

    return result;

}

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_open(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string code = "0x55 FE FE 03 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    DEBUGER.println(code.c_str());
    return RS::RS485::onSvc_set(pld, cmd);        
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_close(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 02";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    return RS::RS485::onSvc_set(pld, cmd);  
}; 
bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_stop(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 03";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_pos(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {    
    std::string code;    
    if (pld->containsKey("pos")) {   
        int pos = (*pld)["pos"];
        if (pos >=0 && pos <=100) {
            code = "0x55 FE FE 03 04" + rfir::util::Util::BytesToHexString((uint8_t*)&pos, 1);        
            code = rfir::util::Crc::Get_CRC16_Str(code);
            (*pld)["code"] = code;
            return RS::RS485::onSvc_set(pld, cmd);    
        }
    } 

    return 0;
};

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_dir(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {    
    std::string code;
    if (pld->containsKey("dir")) {
        int dir = (*pld)["dir"];
        if (dir == 0)
            code = "0x55 FE FE 02 03 01 00";
        if (dir == 1)
            code = "0x55 FE FE 02 03 01 01";
    } 

    if (code != "") {
        code = rfir::util::Crc::Get_CRC16_Str(code);
        (*pld)["code"] = code;
        return RS::RS485::onSvc_set(pld, cmd);     
    }

    return 0;
}; 
bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_up_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 05 01";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_down_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 05 02";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_del_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code = "0x55 FE FE 03 07 FF";
    code = rfir::util::Crc::Get_CRC16_Str(code);
    (*pld)["code"] = code;
    return RS::RS485::onSvc_set(pld, cmd);  
}; 

bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onSvc_set_power_on_prompt(JsonObject* pld, cmds::cmd::CmdBase* cmd)  {
    std::string code;    
    if (pld->containsKey("prompt")) {
        int prompt = (*pld)["prompt"];
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
        (*pld)["code"] = code;
        return RS::RS485::onSvc_set(pld, cmd);     
    }

    return 0; 
}; 
