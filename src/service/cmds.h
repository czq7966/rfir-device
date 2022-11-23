#ifndef SERVICE_CMDS_H
#define SERVICE_CMDS_H

#include "cmds/cmd/cmd.h"

namespace service {
    class Cmds {     
    public:
        void start();
        void loop();
    public:
        int  delay_report_reg_handler = 0;
        int  delay_report_penet_handler = 0;
        void delay_report_reg(int timeout_ms);
        void delay_report_penet(int timeout_ms);
        void do_report_reg();
        void do_report_penet();
        void do_report_penet_offset(int offset);
    public:
        void onCmd(cmds::cmd::RecvCmd* cmd);
        void onCmd_config(cmds::cmd::RecvCmd* cmd);
        void onCmd_handshake(cmds::cmd::RecvCmd* cmd);
        void onCmd_reboot(cmds::cmd::RecvCmd* cmd);
        void onCmd_get(cmds::cmd::RecvCmd* cmd);
        void onCmd_set(cmds::cmd::RecvCmd* cmd);
        void onCmd_report(cmds::cmd::RecvCmd* cmd);
        void onCmd_penet(cmds::cmd::RecvCmd* cmd);
        void onCmd_update(cmds::cmd::RecvCmd* cmd);
        void onCmd_reset_config(cmds::cmd::RecvCmd* cmd);
        void onCmd_device_joined(cmds::cmd::RecvCmd* cmd);
        void onCmd_device_leave(cmds::cmd::RecvCmd* cmd);
        void onCmd_device_interview(cmds::cmd::RecvCmd* cmd);
        void onCmd_get_gpio(cmds::cmd::RecvCmd* cmd);
        void onCmd_set_gpio(cmds::cmd::RecvCmd* cmd);
        void onCmd_rfir_sniff(cmds::cmd::RecvCmd* cmd);
        void onCmd_rfir_send(cmds::cmd::RecvCmd* cmd);       
        void onCmd_intranet(cmds::cmd::RecvCmd* cmd);
    };

}

extern service::Cmds GCmds;     

#endif                
