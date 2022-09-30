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
    doc = new DynamicJsonDocument(Config.props.mqtt_buffer_size);
    root = doc->to<JsonObject>();
    hd = root.createNestedObject("hd");
    pld = root.createNestedObject("pld");
};

cmds::cmd::CmdBase::Command::~Command(){
    delete doc;
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

bool  cmds::cmd::CmdBase::Command::resetHead() {
    head.from.type = "";
    head.from.id = "";
    head.to.type = "";
    head.to.id = "";
    head.entry.type = "";
    head.entry.id = "";
    head.sid = "";
    head.stp = 0;
    return true;
}

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
    root = doc->as<JsonObject>();
    hd = root.containsKey("hd") ? root["hd"] : root.createNestedObject("hd");
    pld = root.containsKey("pld") ? root["pld"] : root.createNestedObject("pld");

    JsonObject from, to, entry;    
    from = hd.containsKey("from") ? hd["from"] : hd.createNestedObject("from");
    to = hd.containsKey("to") ? hd["to"] : hd.createNestedObject("to");
    entry = hd.containsKey("entry") ? hd["entry"] : hd.createNestedObject("entry");
    from["type"] = head.from.type;
    from["id"] = head.from.id;
    to["type"] = head.to.type;
    to["id"] = head.to.id;
    entry["type"] = head.entry.type;
    entry["id"] = head.entry.id;  
    hd["sid"] = head.sid;
    hd["stp"] = head.stp;

};
void cmds::cmd::CmdBase::Command::fixDown(){
    root = doc->as<JsonObject>();
    hd = root.containsKey("hd") ? root["hd"] : root.createNestedObject("hd");
    pld = root.containsKey("pld") ? root["pld"] : root.createNestedObject("pld");

    JsonObject from, to, entry;
    if (hd.containsKey("from")) {
        from = hd["from"];
        head.from.type = from.containsKey("type") ? from["type"] : head.from.type;
        head.from.id = from.containsKey("id") ? from["id"] : head.from.id;
    }

    if (hd.containsKey("to")) {
        to = hd["to"];
        head.to.type = to.containsKey("type") ? to["type"] : head.to.type;
        head.to.id = to.containsKey("id") ? to["id"] : head.to.id;
    }

    if (hd.containsKey("entry")) {
        entry = hd["entry"];
        head.entry.type = entry.containsKey("type") ? entry["type"] : head.entry.type;
        head.entry.id = entry.containsKey("id") ? entry["id"] : head.entry.id;
    }

    head.sid = hd.containsKey("sid") ? hd["sid"] : head.sid;
    head.stp = hd.containsKey("stp") ? hd["stp"] : head.stp;
  
};

void cmds::cmd::CmdBase::Command::fixUpRecv() {
    if (head.to.id == "") {
        head.to.type = cmds::cmd::CmdBase::Command::DefaultFrom->type;
        head.to.id = cmds::cmd::CmdBase::Command::DefaultFrom->id;
    }
};

void cmds::cmd::CmdBase::Command::cloneFrom(Command& cmd){
    head = cmd.head;
    cloneFrom(cmd.hd, cmd.pld);
};
void cmds::cmd::CmdBase::Command::cloneTo(Command& cmd){
    cmd.head = head;
    cloneTo(cmd.hd, cmd.pld);
};

std::string cmds::cmd::CmdBase::Command::toString(){
    fixUp();
    std::string str;
    serializeJson(root,  str);
    return str;

};

void cmds::cmd::CmdBase::Command::fromString(const char* str) {
    deserializeJson(*doc, str);
    fixDown();
}


void cmds::cmd::CmdBase::Command::cloneFrom(JsonObject cmd){
    cloneFrom(cmd["hd"], cmd["pld"]);
};

void cmds::cmd::CmdBase::Command::cloneTo(JsonObject cmd){
    cloneTo(cmd["hd"], cmd["pld"]);
};

void cmds::cmd::CmdBase::Command::cloneFrom(JsonObject hd, JsonObject pld){
    if (!hd.isNull()) hd.set(hd);
    if (!pld.isNull()) pld.set(pld);
    fixDown();
};
void cmds::cmd::CmdBase::Command::cloneTo(JsonObject hd, JsonObject pld){
    fixUp();
    if (!hd.isNull()) hd.set(hd);
    if (!pld.isNull()) pld.set(pld);
};

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
