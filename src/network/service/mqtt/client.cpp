#include "client.h"
#include "rfir/service/cmds/cmd.h"

network::module::mqtt::Client* network::service::mqtt::Client::client = 0;

void network::service::mqtt::Client::Start(network::module::mqtt::Client::Params p) {
    if (!client) 
        client = new network::module::mqtt::Client();
    client->init(p);
    client->start();
    client->onMessage = OnMessage;
}

void network::service::mqtt::Client::Loop() {
    if (client)
        client->loop();
}

bool network::service::mqtt::Client::Publish(const char* payload) {
    if (client)
        return client->publish(payload);
    return 0;
}



void network::service::mqtt::Client::OnMessage(MQTTClient *client, char topic[], char bytes[], int length) {
    Serial.print("OnMessage:");
    Serial.println(bytes);
    neb::CJsonObject jcmd;
    if (jcmd.Parse(bytes)) {
        bool result = 0;
        neb::CJsonObject hd, pld;
        int cmd = 0;                
        if (jcmd.Get("hd", hd) && hd.Get("cmd", cmd)) {
            switch (cmd)
            {
                case 7:
                    hd.ReplaceAdd("stp", 1);
                    hd.ReplaceAdd("did", rfir::util::Util::GetChipId());            
                    jcmd.ReplaceAdd("hd", hd);
                    //jcmd.ReplaceAdd("pld", pld);
                    Publish(jcmd.ToString().c_str());
                    delay(100);
                    result = rfir::service::cmds::Cmd::onCmd(&jcmd);
                    
                    break;    
                case 8:
                case 9:
                case 10:
                case 11:
                    result = rfir::service::cmds::Cmd::onCmd(&jcmd);
                    hd.ReplaceAdd("stp", 1);
                    hd.ReplaceAdd("did", rfir::util::Util::GetChipId());            
                    pld.Add("result", !(int)result);
                    jcmd.ReplaceAdd("hd", hd);
                    jcmd.ReplaceAdd("pld", pld);
                    Publish(jcmd.ToString().c_str());
                    
                    break;
                
                default:
                    break;
            }


        }




    }

    
    
}