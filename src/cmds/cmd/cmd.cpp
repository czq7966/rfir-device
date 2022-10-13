#include "cmd.h"


bool cmds::cmd::Cmd::send(std::list<int> ids){
    
    return 0;
};

bool cmds::cmd::Cmd::recv(const char* buf, int size){
    return 0;
};

cmds::cmd::Cmd GCmdRecv;
cmds::cmd::Cmd GCmdSend;