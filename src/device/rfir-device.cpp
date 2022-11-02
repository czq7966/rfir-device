#include "rfir-device.h"
#include "cmds/cmd/reg-table.h"
#include "rfir/util/debuger.h"


void device::RfirDevice::start(){
    Device::start();
    GTTLSniffer.events.onSniffed.add(this, [this](void* arg, void* p)->void*{
        this->on_rfir_sniffed((rfir::ttl::Sniffer::Data*)p);
        return 0;
    });
};

int device::RfirDevice::on_rfir_sniffed(rfir::ttl::Sniffer::Data* data){
    GDebuger.println(F("device::RfirDevice::on_rfir_sniffed: "));
    GDebuger.print(data->deltas.size());
    GDebuger.println(F(" "));

    int count = data->toBuffer((uint16_t*)this->params.buf);
    if (count > 0) {
        GSendCmd.reset();
        GSendCmd.regTable.tables.add(GRegTable.keys.rfir_sniff_data, (int)this->params.buf);
        GSendCmd.regTable.sizes.add(GRegTable.keys.rfir_sniff_data, count * sizeof(uint16_t));
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::rfir_sniff;
        GSendCmd.send(); 
    }  
    return 0;
};


