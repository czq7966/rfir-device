
#include "Arduino.h"
#include "intranet.h"
#include "WiFiUdp.h"

#include "cmds/cmd/reg-table.h"
#include "cmds/cmd/cmd.h"
#include "network/module/wifi/client.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/debuger.h"

void service::Intranet::init(){
    GWifiClient.events.onWifiConnect.add(this, [this](void* arg, void* p)->void*{
        this->start();        
        return 0;
    });

    GWifiClient.events.onWifiDisconnect.add(this, [this](void* arg, void* p)->void*{
        this->stop();        
        return 0;
    });


};

void service::Intranet::uninit(){
};

void service::Intranet::start(bool init){
    if (init)
        this->init();
    else
        this->start();
};

void service::Intranet::start(){
    if (GRegTable.tables.get(GRegTable.keys.intranet_enable)) {
        stop();
        this->udp = new WiFiUDP();
        IPAddress multicast;
        multicast.fromString(GRegTable.values.intranet_ip);
        uint16_t port = GRegTable.tables.get(GRegTable.keys.intranet_port);
        udp->beginMulticast(WiFi.localIP(), multicast, port);
        
        GEventTimer.delay(100, [this](void* arg, void* p)->void*{
            this->req_intranet();      
            return 0;
        });
    }
};

void service::Intranet::stop(){
    if (udp) {
        udp->stop();
        delete udp;
        udp = 0;
    }
}
void service::Intranet::loop(){
    if (udp)
        this->read();
};

void service::Intranet::read(){
    if (udp->parsePacket()) {
        GDebuger.println(F("service::Intranet::read "));
        int offset = 0;
        while (udp->available())
        {
            offset = offset + udp->read(this->params.buf + offset, this->params.bufsize);
        }
        if (offset) {
            GRecvCmd.recv(this->params.buf, offset);
        }
    }
};

int service::Intranet::write(char* buf, size_t size) {
    if (udp && size) {
        GDebuger.println(F("service::Intranet::write "));
        IPAddress multicast;
        multicast.fromString(GRegTable.values.intranet_ip);
        uint16_t port = GRegTable.tables.get(GRegTable.keys.intranet_port);
        udp->beginPacketMulticast(multicast, port, WiFi.localIP());
        udp->write(buf, size);
        return udp->endPacket();
    } 
    
    return 0;
};

void service::Intranet::dump(char* buf, size_t size){
    GDebuger.println(F("service::Intranet::dump start "));
    for (size_t i = 0; i < size; i++)
    {
        GDebuger.print((uint8_t)buf[i]);
        GDebuger.print(" ");
    }
    GDebuger.println(F(" "));
    GDebuger.println(F("service::Intranet::dump end"));

};

void service::Intranet::req_intranet(){
    GDebuger.println(F("service::Intranet::req_intranet"));
    GSendCmd.reset();
    GSendCmd.head->cmd_id = cmds::cmd::CmdId::intranet;
    GSendCmd.encode();
    this->write((char*)GSendCmd.head, GSendCmd.length());
};

service::Intranet GIntranet;     

            
