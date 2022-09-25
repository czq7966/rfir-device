#ifndef __RFIR_MODULE_DEVICE_NETWORKING_H__
#define __RFIR_MODULE_DEVICE_NETWORKING_H__


#include <Arduino.h>
#include <jled.h>

namespace rfir {
    namespace module {
        namespace device {
            class Networking {
            public:
                struct Status {
                    bool logined = false;
                    bool handshaked = false;
                    bool connected = false;
                };
            public:
                Status status;

                uint32_t m_login_handler = 0;
                uint32_t m_login_reset_handler = 0;
                uint32_t  m_handshake_handler = 0;
                uint32_t  m_handshake_heartbeat_handler = 0;
                int  m_handshake_failed_count = 0;
                int  m_handshake_success_count = 0;
                int  m_online_count = 0;
                int  m_reboot = 1;
            public:
                ~Networking();
                virtual void start();
                virtual void loop();
            public:
                void delayNetworking(int delay_ms = 100);
                void* doNetworking(void* arg, void* p);

                bool handshake();
                void delayHandshake(int delay_ms = 1000);
                void* doHandshake(void* arg, void* p);
                
                bool setWill();
                void setOnline();
                void setReboot();
                void subscribe();
                void unsubscribe();
                void reset();
            public:
                void* onMqttConnect(void* arg, void* p);
                void* onMqttDisconnect(void* arg, void* p);
                void* onNetworkingTimeout(void* arg, void* p);
                void* onCommand(void* arg, void* p);
                void* onConfigFixup(void* arg, void* p);
            public:             
                void* onDev_handshake_resp(void* arg, void* p);                
                void* onDev_handshake_timeout(void* arg, void* p);
            //V3
            public:
                void delayNetworkingV3(int delay_ms = 100);
                void* doNetworkingV3(void* arg, void* p);

            };

        }
    }
}

extern rfir::module::device::Networking GNetworking;

#endif //
