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
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:                    
                    virtual bool onSvc_set_open(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_close(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd);  
                    virtual bool onSvc_set_stop(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_pos(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_dir(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_up_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_down_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_del_travel(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                    virtual bool onSvc_set_power_on_prompt(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd); 
                };
            }
        }
    }
}



#endif //
