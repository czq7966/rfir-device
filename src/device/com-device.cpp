#include "com-device.h"
#include "cmds/cmd/reg-table.h"

void device::ComDevice::start(){
    GRegTable.tables.add(GRegTable.keys.serial_read_timeout, 10);
    this->serial = &Serial;
};

void device::ComDevice::loop(){
    this->read();    
};

size_t device::ComDevice::read() {
    if (this->serial->available()) {
        GSendCmd.reset();
        auto starttime = millis();
        auto timeout = GRegTable.tables.get(GRegTable.keys.serial_read_timeout);
        auto idx = 0;
        auto maxSize = GSendCmd.params.bufsize - sizeof(cmds::cmd::Cmd::Head);
        while (this->serial->available() || millis() - starttime <= timeout)
        {
            if (this->serial->available()) {
                GSendCmd.payload[idx++] = (char)this->serial->read();            
                starttime = millis();
                if (idx >= maxSize)
                    break;
            }
        }

        GSendCmd.head->pld_len = idx;
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::penet;
        Serial.print("device::ComDevice::read size: ");
        Serial.println(GSendCmd.head->pld_len);
        GSendCmd.send();
        return GSendCmd.head->pld_len;
    }
    return 0;
};

size_t device::ComDevice::write(const char* buf, const size_t size) {
    size_t len = 0;
    while (len < size)
    {
        len =  len + this->serial->write(buf + len, size - len);
    }
    return len;
};

int device::ComDevice::onCmd_config(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_handshake(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_reboot(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_get(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_set(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_report(cmds::cmd::RecvCmd* cmd){
    return 0;
};

int device::ComDevice::onCmd_penet(cmds::cmd::RecvCmd* cmd){
    if (cmd->head->pld_len > 0){
        return this->write(cmd->payload, cmd->head->pld_len);
    }
    return 0;
};

int device::ComDevice::onCmd_update(cmds::cmd::RecvCmd* cmd){
    return 0;
};
  