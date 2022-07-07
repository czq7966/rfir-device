#ifndef __RFIR_MODULE_DEVICE_AC_CL_XZ_MSG_GREE_485_H__
#define __RFIR_MODULE_DEVICE_AC_CL_XZ_MSG_GREE_485_H__


#include "../../rs/rs485-modbus.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_XZ_MSG_GREE_485: public RS::RS485_MODBUS {
                public:
                    uint8_t m_addr = 1;
                    std::string getAddrHex();
                public:
                    virtual void init() override;
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:
                    virtual bool onSvc_get_power(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ; 
                    virtual bool onSvc_get_mode(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ; 
                    virtual bool onSvc_get_temp(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ; 
                    virtual bool onSvc_set_power(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ; 
                    virtual bool onSvc_set_mode(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ; 
                    virtual bool onSvc_set_temp(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) ;                     
                };
            }
        }
    }
}



#endif //
