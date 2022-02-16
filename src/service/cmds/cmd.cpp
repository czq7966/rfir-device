#include "cmd.h"
#include "rfir/service/device/device.h"


void service::cmds::Cmd::Start() {

}

void service::cmds::Cmd::Loop() {
    //Device循环
    rfir::service::device::Device::Loop();
}

bool service::cmds::Cmd::PublishMsg(const char* msg) {
    return network::service::mqtt::Client::Publish(msg);
}

void service::cmds::Cmd::DoTimerReport(bool reset) {
    rfir::service::device::Device::DoTimerReport(reset);
}


bool service::cmds::Cmd::OnCmd(const char* cmd) {
    Serial.print("OnCmd: ");
    Serial.println(cmd);
    neb::CJsonObject jcmd;
    if ((int)cmd != 0 &&  jcmd.Parse(cmd)) {
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
            case ECmdId::GetSet: //空调控制   
                return OnCmd_set(jcmd);
                break;            
            case ECmdId::GetVersion: //获取版本号    
                return OnCmd_getversion(jcmd);
                break;   
            case ECmdId::Update: //更新/升级             
            case ECmdId::Reboot: //重启
                return OnCmd_reboot(jcmd, "reboot by user cmd");                
                break;
            case ECmdId::IRSend: //射频发送指令
            case ECmdId::Send:
                return OnCmd_send(jcmd);
                break;  
            case ECmdId::Sniff:
                return OnCmd_sniff(jcmd);
                break;                  
            case ECmdId::Codec:
                return OnCmd_codec(jcmd);
                break;   
            case ECmdId::Config:
                return OnCmd_config(jcmd);
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
    pld.Add("mac", rfir::util::Util::GetMacAddress());
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
    ESP.restart();
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
    pld.Add("mac", rfir::util::Util::GetMacAddress());

    cmd.Add("hd", hd);
    cmd.Add("pld", pld);    
    return PublishMsg(cmd.ToString().c_str());
}

bool service::cmds::Cmd::OnCmd_set(neb::CJsonObject* cmd) {
    neb::CJsonObject pld;
    std::string key;

    if (!cmd->Get("pld", pld) || !pld.GetKey(key))
        return OnCmd_get(cmd);
    else {
        auto result = rfir::service::device::Device::OnCmd_set(&pld);
        DoTimerReport(true);
        return result;
    }
}

bool service::cmds::Cmd::OnCmd_get(neb::CJsonObject* _doc, std::string reason) {
    neb::CJsonObject cmd, hd, pld;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", ECmdId::GetSet);
    hd.Add("stp", 1);
    rfir::service::device::Device::OnCmd_get(&pld);
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

bool service::cmds::Cmd::OnCmd_sniff(neb::CJsonObject* cmd) {
    auto rfir = rfir::GetRfir();
    rfir->sniffer->stop();
    rfir::service::cmds::Cmd::onCmd(cmd); 
    rfir->sniffer->start();
    return true;
}


bool service::cmds::Cmd::OnCmd_codec(neb::CJsonObject* cmd) {
    return rfir::service::cmds::Cmd::onCmd(cmd); 
}

bool service::cmds::Cmd::OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (rfir::service::device::Device::OnCmd_decoded(data)) {
        OnCmd_get(0, "IR Change");
        
        //Report
        DoTimerReport(true);
        return true;
    }
    return false;
}


bool service::cmds::Cmd::OnCmd_config(neb::CJsonObject* cmd) {
    neb::CJsonObject pld, content;
    std::string fn;
    if (cmd->Get("pld", pld)) {
        if (pld.Get("filename", fn)) {

        }
    }
        return OnCmd_getconfig(&pld);
    return false;
}

bool service::cmds::Cmd::OnCmd_getconfig(neb::CJsonObject* cmd) {
    neb::CJsonObject pld;
    if (cmd->Get("pld", pld))
        return rfir::service::device::Device::OnCmd_config(&pld);
    return false;
}