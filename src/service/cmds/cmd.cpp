#include "cmd.h"
// #include "../../module/ac/mcquay.h"


bool service::cmds::Cmd::PublishMsg(const char* msg) {
    return network::service::mqtt::Client::Publish(msg);
}



bool service::cmds::Cmd::OnCmd(const char* cmd) {
    Serial.print("OnCmd: ");
    Serial.println(cmd);
    neb::CJsonObject jcmd;
    if (jcmd.Parse(cmd)) {
        return OnCmd(&jcmd);
    }
    return false;     
}

bool service::cmds::Cmd::OnCmd(neb::CJsonObject* jcmd) {
    neb::CJsonObject hd;
    int cmd = 0;                
    if (jcmd->Get("hd", hd) && hd.Get("cmd", cmd)) {
        switch (cmd)
        {
            case ECmdId::HeartBeet: //心跳        
                return OnCmd_heartbeat(jcmd);
                break;
            case ECmdId::ACSet: //空调控制   
                return OnCmd_ac_set(jcmd);
                break;            
            case ECmdId::GetVersion: //获取版本号    
                return OnCmd_getversion(jcmd);
                break;   
            case ECmdId::Update: //更新/升级             
            case ECmdId::Reboot: //重启
                return OnCmd_reboot(jcmd, "reboot by user cmd");                
                break;
            case ECmdId::Send:
                return OnCmd_send(jcmd);
                break;  
            case ECmdId::Codec:
                return OnCmd_Codec(jcmd);
                break;                            
            default:
                break;
        }        
    }
    
    return false;
}

bool service::cmds::Cmd::OnCmd_heartbeat(neb::CJsonObject* _cmd, uint8_t st) {
    neb::CJsonObject cmd, hd, pld;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", ECmdId::HeartBeet);
    hd.Add("stp", 1);
    pld.Add("st", st);
    pld.Add("ver", OTA_VERSION_NUMBER);
    pld.Add("rssi", WiFi.RSSI());
    cmd.Add("hd", hd);
    cmd.Add("pld", pld);
    
    return PublishMsg(cmd.ToString().c_str());
}

bool service::cmds::Cmd::OnCmd_reboot(neb::CJsonObject* _doc, std::string reason) {
    neb::CJsonObject cmd, hd, pld;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", ECmdId::Reboot);
    hd.Add("stp", 1);

    pld.Add("extra", reason);

    cmd.Add("hd", hd);
    cmd.Add("pld", pld);
    
    PublishMsg(cmd.ToString().c_str());

    delay(5000);
#ifdef ESP8266    
    ESP.reset();
#else 
    ESP.retart();
#endif
    delay(1000);
    return true;
}


bool service::cmds::Cmd::OnCmd_getversion(neb::CJsonObject* _doc) {
    neb::CJsonObject cmd, hd, pld;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", ECmdId::GetVersion);
    hd.Add("stp", 1);

    pld.Add("version", OTA_VERSION_NUMBER);
    pld.Add("rssi", WiFi.RSSI());

    cmd.Add("hd", hd);
    cmd.Add("pld", pld);    
    return PublishMsg(cmd.ToString().c_str());
}

bool service::cmds::Cmd::OnCmd_ac_set(neb::CJsonObject* cmd) {
    neb::CJsonObject pld;
    std::string key;

    if (!cmd->Get("pld", pld) || !pld.GetKey(key))
        return OnCmd_ac_get(cmd);
    
    auto ac = module::ac::Mcquay::AC;

    //Power
    auto power = module::ac::Mcquay::GetPower();
    std::string powerStr;
    pld.Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac->getMode();
    std::string modeStr;
    pld.Get("mode", modeStr);
    mode =  modeStr == "cool" ? module::ac::McquayAC::KMcQuayModeCool : 
            modeStr == "dry" ? module::ac::McquayAC::KMcQuayModeDry : 
            modeStr == "fan" ? module::ac::McquayAC::KMcQuayModeFan : 
            modeStr == "fan_only" ? module::ac::McquayAC::KMcQuayModeFan : 
            modeStr == "heat" ? module::ac::McquayAC::KMcQuayModeHeat : mode;

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr;
    pld.Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? module::ac::McquayAC::KMcQuayFanAuto : 
            fanStr == "low" ? module::ac::McquayAC::KMcQuayFanMin : 
            fanStr == "medium" ? module::ac::McquayAC::KMcQuayFanMed : 
            fanStr == "high" ? module::ac::McquayAC::KMcQuayFanMax : fan;

    //Sleep
    auto sleep = ac->getSleep();
    std::string sleepStr;
    pld.Get("sleep", sleepStr);
    sleep = sleepStr == "on" ? true : 
            sleepStr == "off" ? false : sleep;

    //Swing
    auto swing = ac->getSwing();
    std::string swingStr;
    pld.Get("swing", swingStr);
    swing = swingStr == "on" ? true : 
            swingStr == "off" ? false : swing;

    //Temp
    uint32 temp = ac->getTemp();
    pld.Get("temperature", temp);

    //小时
    uint32 hour = ac->getHour();
    if (!pld.Get("hour", hour)) 
        hour = millis() / 1000 / 60 / 60 % 24;    
    //分钟
    uint32 minute = ac->getMinute();
    if (!pld.Get("minute", minute))
        minute = millis() / 1000 / 60 % 60;


    ac->setMode(mode);
    ac->setFan(fan);
    ac->setSleep(sleep);
    ac->setSwing(swing);
    ac->setTemp(temp);
    ac->setHour(hour);
    ac->setMinute(minute);
    ac->setPowerSwitch(power != module::ac::Mcquay::GetPower());
    ac->setRaw(ac->getRaw());

    auto raw = ac->getEncodeRaw();
    auto rfir = rfir::GetRfir();
    rfir->sniffer->stop();
    rfir->sender->sendRaw(raw, module::ac::McquayAC::KMcQuayEncodeRawLength);
    rfir->sniffer->start();
    
    //Report
    module::ac::Mcquay::DoTimerReport(true);

    return true;
}

bool service::cmds::Cmd::OnCmd_ac_get(neb::CJsonObject* _doc, std::string reason) {
    neb::CJsonObject cmd, hd, pld;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", ECmdId::ACSet);
    hd.Add("stp", 1);

    pld.Parse(module::ac::Mcquay::ToString().c_str()); 
    pld.Add("reason", reason);

    cmd.Add("hd", hd);
    cmd.Add("pld", pld);    
    return PublishMsg(cmd.ToString().c_str());
}

bool service::cmds::Cmd::OnCmd_send(neb::CJsonObject* cmd) {
    auto rfir = rfir::GetRfir();
    rfir->sniffer->stop();
    rfir::service::cmds::Cmd::onCmd(cmd); 
    rfir->sniffer->start();
    return true;
}



bool service::cmds::Cmd::OnCmd_Codec(neb::CJsonObject* cmd) {
    return rfir::service::cmds::Cmd::onCmd(cmd); 
}

bool service::cmds::Cmd::On_Decoded(uint8_t* bytes, uint16_t nbits) {
    module::ac::Mcquay::AC->setRaw(bytes);
    OnCmd_ac_get(0, "IR Change");
    
    //Report
    module::ac::Mcquay::DoTimerReport(true);
    return true;
}