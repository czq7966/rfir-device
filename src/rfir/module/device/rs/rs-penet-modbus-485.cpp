#include "rs-penet-modbus-485.h"
#include "rfir/util/crc.h"
#include "cmds/cmd/cmd-mqtt.h"


void rfir::module::device::RS::RSPenetModbus485::init() {
    RS::RSPenet::init();
    this->name = "RSPenetModbus";    
}

bool rfir::module::device::RS::RSPenetModbus485::checkSumCode(uint8_t* rx_buf, uint8_t len) {
    bool result = false;
    if (len > 2) {
        uint8_t crcLow;
        uint8_t crcHigh;
        rfir::util::Crc::Get_CRC16(rx_buf, len - 2, crcLow, crcHigh);
        result = (rx_buf[len - 1] == crcHigh && rx_buf[len - 2] == crcLow);
    }  

    return result;
}; 

bool rfir::module::device::RS::RSPenetModbus485::onSvc_penet(neb::CJsonObject* p_pld, cmds::cmd::CmdBase* reqCmd) {
    auto result = RS::RSPenet::onSvc_penet(p_pld, reqCmd);
    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;    
    if (result) {
        std::string code = rsReadBase64(rsBuffer);
        if (code.length() > 0) {
            pld.Add("raw", code);
            result = true;            
        } else {
            pld.ReplaceAdd("_reason", "recv code size == 0 or checksum invalid");
            result = false;
        }
    }



    pld.ReplaceAdd("_success", result);

    if (reqCmd){
        hd = reqCmd->command.hd;
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    }

    result = cmd.send();
    return result;

}
