#include "networking.h"
#include "network/module/mqtt/async-client.h"
#include "cmds/cmd/cmd.h"

//事件注册
void service::Networking::start(){
    GMqttClient.events.onMqttConnect.add(this, [this](void* arg, void* p)-> void*{
        return 0;
    });
    GMqttClient.events.onMqttDisconnect.add(this, [this](void* arg, void* p)-> void*{
        return 0;
    });
    GMqttClient.events.onMqttMessage.add(this, [this](void* arg, void* p)-> void*{
        auto msg = (::network::module::mqtt::AClient::Message*)p;
        if (msg->total < (sizeof(cmds::cmd::Cmd::Head) - sizeof(char*)) )
            return 0;

        GCmdRecv.recv(msg->payload, msg->len);

        return 0;
    });

    GCmdRecv.events.recv.add(this, [this](void* arg, void* p)-> void*{
        return 0;
    });

    GCmdSend.events.send.add(this, [this](void* arg, void* p)-> void*{

        return 0;
    });

    //热点逻辑
    this->ap.start();

};


void service::Networking::loop(){

}

service::Networking GNetworking;