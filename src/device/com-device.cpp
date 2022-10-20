#include "com-device.h"
#include "cmds/cmd/reg-table.h"

void device::ComDevice::start(){
    GRegTable.tables.add(GRegTable.keys.serial_read_timeout, 10);
    this->bufsize = GRegTable.tables.get(GRegTable.keys.serial_read_bufsize);
    this->buf = new char[this->bufsize];
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

        while (this->serial->available() || millis() - starttime <= timeout)
        {
            if (this->serial->available()) {
                this->buf[idx++] = (char)this->serial->read();            
                starttime = millis();
                if (idx >= this->bufsize)
                    break;
            }
        }
        if (idx > 0) {
            GSendCmd.regTable.tables.add(GRegTable.keys.penet_data, (int)this->buf);
            GSendCmd.regTable.sizes.add(GRegTable.keys.penet_data, idx);

            GSendCmd.head->cmd_id = cmds::cmd::CmdId::penet;
            GSendCmd.send();
            return idx;
        }
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
        auto data = cmd->regTable.tables.get(GRegTable.keys.penet_data);
        auto size = cmd->regTable.sizes.get(GRegTable.keys.penet_data);
        if (data && size) {
            return this->write((char*)data, size);
        }
    }
    return 0;
};

int device::ComDevice::onCmd_update(cmds::cmd::RecvCmd* cmd){
    return 0;
};
  