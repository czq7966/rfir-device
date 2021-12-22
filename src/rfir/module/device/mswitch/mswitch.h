#ifndef __RFIR_MODULE_DEVICE_MSWITCH_MSwitch_H__
#define __RFIR_MODULE_DEVICE_MSWITCH_MSwitch_H__


#include "../device.h"
#include "switch.h"
#include <map>

namespace rfir {
    namespace module {
        namespace device {
            namespace mswitch {
                class MSwitch: public Device {
                public:
                    std::map<std::string, Switch*> switchs;
                public:
                    ~MSwitch();
                    bool addSwitch(Switch* switch_p);
                    bool removeSwitch(Switch* switch_p);
                    void destroySwitchs();
                    
                    bool doSwitchChange();                    
                public:
                    virtual void start(void *) override;
                    virtual void loop() override; 

                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override;                     
                };
            }
        }
    }
}



#endif //
