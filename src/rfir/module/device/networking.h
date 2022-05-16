#ifndef __RFIR_MODULE_DEVICE_NETWORKING_H__
#define __RFIR_MODULE_DEVICE_NETWORKING_H__


#include <Arduino.h>

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
                uint32_t  m_handshake_handler = 0;
                int  m_handshake_failed_count = 0;
                int  m_handshake_success_count = 0;
                int  m_online_count = 0;
            public:
                ~Networking();
                virtual void start();
                virtual void loop();
            public:
                bool login();
                void delayLogin(int delay_ms = 3000);
                void* doLogin(void* arg, void* p);
                bool handshake();
                void delayHandshake(int delay_ms = 1000);
                void* doHandshake(void* arg, void* p);
                
                bool setWill();
                void setOnline();
                void subscribe();
                void unsubscribe();
            public:
                void* onConnect(void* arg, void* p);
                void* onDisconnect(void* arg, void* p);
                void* onCommand(void* arg, void* p);
                void* onConfigFixup(void* arg, void* p);
            public:
                void* onDev_login_resp(void* arg, void* p);
                void* onDev_login_timeout(void* arg, void* p);
                void* onDev_handshake_resp(void* arg, void* p);                
                void* onDev_handshake_timeout(void* arg, void* p);

                void* onDsp_status_change(void* arg, void* p);
                void* onEdg_status_change(void* arg, void* p);
            };

        }
    }
}

extern rfir::module::device::Networking GNetworking;

#endif //
