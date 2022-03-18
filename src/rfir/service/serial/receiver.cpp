#include "receiver.h"
#include "../cmds/cmd.h"

void rfir::service::serial::Receiver::dealCmd() {
    String cmd;
    neb::CJsonObject json;
    int c = DEBUGER.read();
    if (c >= 0) {
        cmd += (char)c;
        
        while (true) {
            String str = DEBUGER.readStringUntil('\n');
            if (str.length() == 0)
                break;

            cmd += str;
            if (json.Parse(cmd.c_str())) {                
                cmd = "";
                cmds::Cmd::onCmd(&json);
            }
        }
    }
}



