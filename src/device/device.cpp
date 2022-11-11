#include "device.h"
#include "rfir/ttl/sender.h"
#include "rfir/ttl/sniffer.h"
#include "rfir/util/debuger.h"
#include "rfir/util/interrupt.h"
#include "cmds/cmd/reg-table.h"

void device::Device::start(){

};

void device::Device::loop(){
    
};


int device::Device::onCmd_config(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_handshake(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_reboot(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_get(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_set(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_report(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_penet(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::Device::onCmd_update(cmds::cmd::RecvCmd* cmd){
    return 0;
};
  
int device::Device::onCmd_reset_config(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_device_joined(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_device_leave(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_device_interview(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_get_gpio(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_set_gpio(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
int device::Device::onCmd_rfir_sniff(cmds::cmd::RecvCmd* cmd){
    GTTLSniffer.params.enabled = GRegTable.get(GRegTable.keys.rfir_sniff_enable, &cmd->regTable);
    GTTLSniffer.params.pin = GRegTable.get(GRegTable.keys.rfir_sniff_pin, &cmd->regTable);
    GTTLSniffer.params.inverted = GRegTable.get(GRegTable.keys.rfir_sniff_inverted, &cmd->regTable);
    GTTLSniffer.params.minCount = GRegTable.get(GRegTable.keys.rfir_sniff_minCount, &cmd->regTable);
    GTTLSniffer.params.maxCount = GRegTable.get(GRegTable.keys.rfir_sniff_maxCount, &cmd->regTable);
    GTTLSniffer.params.minDelta = GRegTable.get(GRegTable.keys.rfir_sniff_minDelta, &cmd->regTable);
    GTTLSniffer.params.maxDelta = GRegTable.get(GRegTable.keys.rfir_sniff_maxDelta, &cmd->regTable);
    GTTLSniffer.stop();
    if (GTTLSniffer.params.enabled) {
        GTTLSniffer.start();
        GTTLSniffer.startSniff();
        return 1;
    }

    return 0;
};
    
int device::Device::onCmd_rfir_send(cmds::cmd::RecvCmd* cmd){
    GDebuger.print("device::Device::onCmd_rfir_send: ");
    GDebuger.println(cmd->head->cmd_id);
    GTTLSender.params.enabled = GRegTable.get(GRegTable.keys.rfir_send_enable, &cmd->regTable);
    GTTLSender.params.pin = GRegTable.get(GRegTable.keys.rfir_send_pin, &cmd->regTable);
    GTTLSender.params.inverted = GRegTable.get(GRegTable.keys.rfir_send_inverted, &cmd->regTable);
    GTTLSender.params.modulation = GRegTable.get(GRegTable.keys.rfir_send_modulation, &cmd->regTable);
    GTTLSender.params.repeat = GRegTable.get(GRegTable.keys.rfir_send_repeat, &cmd->regTable);
    GTTLSender.params.frequency = GRegTable.get(GRegTable.keys.rfir_send_frequency, &cmd->regTable);
    GTTLSender.params.dutycycle = GRegTable.get(GRegTable.keys.rfir_send_dutycycle, &cmd->regTable);
    auto data = cmd->regTable.tables.get(GRegTable.keys.rfir_send_data);
    auto size = cmd->regTable.sizes.get(GRegTable.keys.rfir_send_data);
    if (data && size) {
        GInterrupt.stop();
        GTTLSender.sendRaw((uint16_t*)data, size / 2);
        GInterrupt.start();
        return size / 2;
    }

    return 0;
};

int device::Device::onCmd_intranet(cmds::cmd::RecvCmd* cmd){
    return 0;
};
    
