#ifndef __RFIR_MODULE_DEVICE_CURTAIN_CL_DJROOM_DOOYA_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_CL_DJROOM_DOOYA_H__


#include "../../rs/rs485.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace curtain {
                class CL_DJROOM_Dooya_485: public RS::RS485 {
                public:
                    virtual void init() override;
                    virtual int onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_set(JsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:                    
                    virtual bool onSvc_set_open(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_close(JsonObject* pld, cmds::cmd::CmdBase* cmd);  
                    virtual bool onSvc_set_stop(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_pos(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_dir(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_up_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_down_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_del_travel(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_power_on_prompt(JsonObject* pld, cmds::cmd::CmdBase* cmd); 
                };
            }
        }
    }
}



#endif //
