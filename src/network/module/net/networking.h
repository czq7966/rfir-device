// #ifndef _NETWORK_MODULE_NET_NETWORKING_H_
// #define _NETWORK_MODULE_NET_NETWORKING_H_

// #include "../mqtt/async-client.h"
// namespace network {
//     namespace module {
//         namespace net {
//             class Networking {
//             public:
//                 bool logined = false;
//                 network::module::mqtt::AClient* mqtt = 0;
//             public:
//                 void start();
//                 void loop();
//                 void login();
//             public:
//                 void* onConnected(void* arg, void* p);
//                 void* onLoginResp(void* arg, void* p);
//                 void* onLoginTimeout(void* arg, void* p);
//             };

//         }
//     }
// }

// // extern network::module::net::Networking GNetworking;

// #endif