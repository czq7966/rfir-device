#ifndef __RFIR_MODULE_DEVICE_NETWORKING_H__
#define __RFIR_MODULE_DEVICE_NETWORKING_H__


#include <Arduino.h>
#include <jled.h>
#include "ap.h"
#include "cmds/cmd/cmd.h"

namespace service {
    class Networking {
    public:
        uint32_t m_handshake_andler = 0;
        cmds::cmd::Cmd::Head willPayload;
    public:
        virtual void start();
        virtual void loop();
    public:
        bool handshake(cmds::cmd::Cmd* cmd = nullptr);
        void delayHandshake(int delay_ms = 1000);
        
        void setWill();
        void setOnline();
        void subscribe();
    public:
        int  publish(const char* topic, const char* payload = nullptr, size_t length = 0, uint8_t qos = 2, bool retain = false, bool dup = false, uint16_t message_id = 0);
        int  setWill(const char* topic, const char* payload = nullptr, size_t length = 0, uint8_t qos = 2, bool retain = true);

    };

}


extern service::Networking GNetworking;

#endif //
