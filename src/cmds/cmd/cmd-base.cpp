#include "cmd-base.h"
#include "rfir/util/event-timer.h"
#include "config.h"

uint32_t& cmds::cmd::CmdBase::Command::SessionID = rfir::util::EventTimer::HandlerIndex;
cmds::cmd::CmdBase::Command::Address* cmds::cmd::CmdBase::Command::DefaultFrom = new cmds::cmd::CmdBase::Command::Address();
cmds::cmd::CmdBase::Command::Address* cmds::cmd::CmdBase::Command::DefaultTo = new cmds::cmd::CmdBase::Command::Address();
int cmds::cmd::CmdBase::Command::DefaultRespTimeout = 10 * 1000;


cmds::cmd::CmdBase::Command::Command(){
    head.from.type = DefaultFrom->type;
    head.from.id = DefaultFrom->id;
    head.to.type = DefaultTo->type;
    head.to.id = DefaultTo->id;    
};

std::string cmds::cmd::CmdBase::Command::getSid(){
    return head.sid;
};
uint32_t cmds::cmd::CmdBase::Command::getIntSid(){
    return atoi(getSid().c_str());
};
bool cmds::cmd::CmdBase::Command::setSid(std::string value){
    head.sid = value;
    return 1;
};
bool cmds::cmd::CmdBase::Command::setSid(uint32_t value){
    return setSid(String(value).c_str());
};
bool cmds::cmd::CmdBase::Command::isReqCmd(){
    return head.stp == 0;
};
bool cmds::cmd::CmdBase::Command::isRespCmd(){
    return head.stp == 1;
};
bool cmds::cmd::CmdBase::Command::setReqCmd(){
    head.stp = 0;
    return 1;
};
bool cmds::cmd::CmdBase::Command::setRespCmd(){
    head.stp = 1;
    return 1;
};

bool cmds::cmd::CmdBase::Command::isNeedResp(){
    return isReqCmd() && (getIntSid() > 0 || getSid() != "");
};
bool cmds::cmd::CmdBase::Command::setNeedResp(bool value){
    if (value) {
        SessionID++;
        setSid(SessionID);
    } else {
        setSid(0);
    }
    return 1;
};

void cmds::cmd::CmdBase::Command::fixUp(){
    neb::CJsonObject from, to, entry;
    from.Add("type", head.from.type);
    from.Add("id", head.from.id);
    to.Add("type", head.to.type);
    to.Add("id", head.to.id);
    entry.Add("type", head.entry.type);
    entry.Add("id", head.entry.id);

    hd.ReplaceAdd("from", from);
    hd.ReplaceAdd("to", to);
    hd.ReplaceAdd("entry", entry);
    hd.ReplaceAdd("sid", head.sid);
    hd.ReplaceAdd("stp", head.stp);
};
void cmds::cmd::CmdBase::Command::fixDown(){
    neb::CJsonObject from, to, entry;
    if (hd.Get("from", from)) {
        from.Get("type", head.from.type);
        from.Get("id", head.from.id);
    }

    if (hd.Get("to", to)) {
        to.Get("type", head.to.type);
        to.Get("id", head.to.id);
    }

    if (hd.Get("entry", entry)) {
        entry.Get("type", head.entry.type);
        entry.Get("id", head.entry.id);
    }

    hd.Get("sid", head.sid);
    hd.Get("stp", head.stp);    
};

void cmds::cmd::CmdBase::Command::cloneFrom(Command& cmd){
    head = cmd.head;
    cloneFrom(cmd.hd, cmd.pld);
};
void cmds::cmd::CmdBase::Command::cloneTo(Command& cmd){
    cmd.head = head;
    cloneTo(cmd.hd, cmd.pld);
};
void cmds::cmd::CmdBase::Command::cloneFrom(neb::CJsonObject& cmd) {
    cmd.Get("hd", hd);
    cmd.Get("pld", pld);
    fixDown();
};
void cmds::cmd::CmdBase::Command::cloneTo(neb::CJsonObject& cmd){
    fixUp();
    cmd.ReplaceAdd("hd", hd);
    cmd.ReplaceAdd("pld", pld);
};

void cmds::cmd::CmdBase::Command::cloneFrom(neb::CJsonObject& _hd, neb::CJsonObject& _pld){
    hd.Parse(_hd.ToString());
    pld.Parse(_pld.ToString());
    fixDown();
};
void cmds::cmd::CmdBase::Command::cloneTo(neb::CJsonObject& _hd, neb::CJsonObject& _pld){
    fixUp();
    _hd.Parse(hd.ToString());
    _pld.Parse(pld.ToString());
};

std::string cmds::cmd::CmdBase::Command::toString(){
    neb::CJsonObject cmd;
    cloneTo(cmd);
    return cmd.ToString();
};

void cmds::cmd::CmdBase::Command::fromString(const char* str) {
    neb::CJsonObject cmd(str);
    cloneFrom(cmd);
}

// void cmds::cmd::CmdBase::Events::cloneFrom(Events& events){
//     onResp = events.onResp;
//     onTimeout = events.onTimeout;
// };
// void cmds::cmd::CmdBase::Events::cloneTo(Events& events){
//     events.onResp = onResp;
//     events.onTimeout = onTimeout;
// };     

// cmds::cmd::CmdBase::Events* cmds::cmd::CmdBase::Events::clone(){
//     auto events = new Events();
//     events->cloneFrom(*this);
//     return events;
// };

bool cmds::cmd::CmdBase::send(const void* p){
    return false;
};
void cmds::cmd::CmdBase::cloneFrom(CmdBase& cmd){
    command.cloneFrom(cmd.command);
    events = cmd.events;
};
void cmds::cmd::CmdBase::cloneTo(CmdBase& cmd){
    command.cloneTo(cmd.command);
    cmd.events = events;
};   
