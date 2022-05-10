#ifndef __RFIR_MODULE_DEVICE_NETWORKING_H__
#define __RFIR_MODULE_DEVICE_NETWORKING_H__


#include <Arduino.h>

namespace rfir {
    namespace module {
        namespace device {
            class Networking {
            public:
                bool m_logined = false;
                uint32_t m_login_handler = 0;
                uint32_t  m_handshake_handler = 0;
                int  m_handshake_failed_count = 0;
                int  m_handshake_success_count = 0;
            public:
                ~Networking();
                virtual void start();
                virtual void loop();
            public:
                bool login();
                void delayLogin(int delay_ms = 3000);
                bool handshake();
                bool setWill();
                void setOnline();
                void subscribe();
            public:
                void* onConnect(void* arg, void* p);
                void* onDisconnect(void* arg, void* p);
                void* onCommand(void* arg, void* p);
                void* onConfigFixup(void* arg, void* p);
            public:
                void* doLogin(void* arg, void* p);
                void* onLoginReq(void* arg, void* p);
                void* onLoginResp(void* arg, void* p);
                void* onLoginTimeout(void* arg, void* p);
                void* onHandshakeReq(void* arg, void* p);
                void* onHandshakeResp(void* arg, void* p);                
                void* onHandshakeTimeout(void* arg, void* p);
                void* onEdgSatusChange(void* arg, void* p);
            };

        }
    }
}

extern rfir::module::device::Networking GNetworking;

#endif //
