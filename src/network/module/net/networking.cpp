// #include "networking.h"
// #include "../mqtt/async-client.h"
// #include "rfir/util/util.h"
// #include "cmds/cmd/cmd-mqtt.h"

// void network::module::net::Networking::start() {
//     mqtt = &GMqttClient;
//     mqtt->events.onMqttConnect.add((void*)this, std::bind(&Networking::onConnected, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    
//     // mqtt->events.on(network::module::mqtt::Client::Events::OnConnected, (void*)OnConnected, OnConnected, (void*)this);
//     // mqtt->topicEvents.on(Config.mqtt_net_lgn_rsp_topic, (void*)OnLoginResp, OnLoginResp,  (void*)this);
// };

// void network::module::net::Networking::loop() {

// };

// void network::module::net::Networking::login() {
//     // cmds::cmd::CmdMqtt cmd;
//     // cmd.send(Config.mqtt_net_lgn_topic.c_str());

//     // neb::CJsonObject msg, hd;
//     // hd.Add("id", rfir::util::Util::GetChipId(CHIP_ID_PREFIX));
//     // hd.Add("version", OTA_VERSION_NUMBER);
//     // hd.Add("rssi", WiFi.RSSI());
//     // hd.Add("ssid", WiFi.SSID().c_str());
//     // hd.Add("ip", WiFi.localIP().toString().c_str());
//     // hd.Add("mac", rfir::util::Util::GetMacAddress());
//     // hd.Add("version", OTA_VERSION_NUMBER);
//     // hd.Add("facturer", DEV_FACTURER);
//     // hd.Add("model", DEV_MODEL);
//     // msg.Add("hd", hd);
    
//     // this->mqtt->publish(Config.mqtt_net_lgn_topic.c_str(), msg.ToString().c_str());
// }


// void* network::module::net::Networking::onConnected(void* arg, void* p) {
//     cmds::cmd::CmdMqtt cmd;
//     cmd.command.
//     mqtt->mqtt.setWill();

//     auto networking = (Networking*)arg;
//     auto msg = (network::module::mqtt::Client::Message*)p;
//     if (!networking->logined) {
//         networking->login();
//     }
//     return 0;
// };

// void* network::module::net::Networking::OnLoginResp(void* arg, void* p) {
//     auto networking = (Networking*)arg;
//     auto msg = (network::module::mqtt::Client::Message*)p;

//     return 0;

// };


// network::module::net::Networking GNetworking;