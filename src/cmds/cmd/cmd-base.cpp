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
    doc = new DynamicJsonDocument(MQTT_BUFFER_SIZE);
    root = doc->to<JsonObject>();
    jhd = root.createNestedObject("hd");
    jpld = root.createNestedObject("pld");
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
    jhd = root.containsKey("hd") ? root["hd"] : root.createNestedObject("hd");
    jpld = root.containsKey("pld") ? root["pld"] : root.createNestedObject("pld");

    JsonObject from, to, entry;    
    from = jhd.containsKey("from") ? jhd["from"] : jhd.createNestedObject("from");
    to = jhd.containsKey("to") ? jhd["to"] : jhd.createNestedObject("to");
    entry = jhd.containsKey("entry") ? jhd["entry"] : jhd.createNestedObject("entry");
    from["type"] = head.from.type;
    from["id"] = head.from.id;
    to["type"] = head.to.type;
    to["id"] = head.to.id;
    entry["type"] = head.entry.type;
    entry["id"] = head.entry.id;  
    jhd["sid"] = head.sid;
    jhd["stp"] = head.stp;


    // if (jhd.containsKey("from")) {
    //     JsonObject from = jhd["from"];
    //     head.from.type = from.containsKey("type") ? from["type"] : head.from.type;
    //     head.from.id = from.containsKey("id") ? from["id"] : head.from.id;
    // }


    // neb::CJsonObject from, to, entry;
    // hd.Get("from", from);
    // hd.Get("to", to);
    // hd.Get("entry", entry);

    // from.ReplaceAdd("type", head.from.type);
    // from.ReplaceAdd("id", head.from.id);
    // to.ReplaceAdd("type", head.to.type);
    // to.ReplaceAdd("id", head.to.id);
    // entry.ReplaceAdd("type", head.entry.type);
    // entry.ReplaceAdd("id", head.entry.id);

    // hd.ReplaceAdd("from", from);
    // hd.ReplaceAdd("to", to);
    // hd.ReplaceAdd("entry", entry);
    // hd.ReplaceAdd("sid", head.sid);
    // hd.ReplaceAdd("stp", head.stp);
};
void cmds::cmd::CmdBase::Command::fixDown(){
    root = doc->as<JsonObject>();
    jhd = root.containsKey("hd") ? root["hd"] : root.createNestedObject("hd");
    jpld = root.containsKey("pld") ? root["pld"] : root.createNestedObject("pld");

    JsonObject from, to, entry;
    if (jhd.containsKey("from")) {
        from = jhd["from"];
        head.from.type = from.containsKey("type") ? from["type"] : head.from.type;
        head.from.id = from.containsKey("id") ? from["id"] : head.from.id;
    }

    if (jhd.containsKey("to")) {
        to = jhd["to"];
        head.to.type = to.containsKey("type") ? to["type"] : head.to.type;
        head.to.id = to.containsKey("id") ? to["id"] : head.to.id;
    }

    if (jhd.containsKey("entry")) {
        entry = jhd["entry"];
        head.entry.type = entry.containsKey("type") ? entry["type"] : head.entry.type;
        head.entry.id = entry.containsKey("id") ? entry["id"] : head.entry.id;
    }

    head.sid = jhd.containsKey("sid") ? jhd["sid"] : head.sid;
    head.stp = jhd.containsKey("stp") ? jhd["stp"] : head.stp;

    // neb::CJsonObject from, to, entry;
    // if (hd.Get("from", from)) {
    //     from.Get("type", head.from.type);
    //     from.Get("id", head.from.id);
    // }

    // if (hd.Get("to", to)) {
    //     to.Get("type", head.to.type);
    //     to.Get("id", head.to.id);
    // }

    // if (hd.Get("entry", entry)) {
    //     entry.Get("type", head.entry.type);
    //     entry.Get("id", head.entry.id);
    // }

    // hd.Get("sid", head.sid);
    // hd.Get("stp", head.stp);    
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
    fixUp();
    std::string str;
    serializeJson(root,  str);
    return str;
    // neb::CJsonObject cmd;
    // cloneTo(cmd);
    // return cmd.ToString();
};

void cmds::cmd::CmdBase::Command::fromString(const char* str) {
    deserializeJson(*doc, str);
    fixDown();

    // neb::CJsonObject cmd(str);
    // cloneFrom(cmd);
}


void cmds::cmd::CmdBase::Command::cloneFrom(JsonObject cmd){
    cloneFrom(cmd["hd"], cmd["pld"]);
};

void cmds::cmd::CmdBase::Command::cloneTo(JsonObject cmd){
    cloneTo(cmd["hd"], cmd["pld"]);
};

void cmds::cmd::CmdBase::Command::cloneFrom(JsonObject hd, JsonObject pld){
    if (!hd.isNull()) jhd.set(hd);
    if (!pld.isNull()) jpld.set(pld);
    fixDown();
};
void cmds::cmd::CmdBase::Command::cloneTo(JsonObject hd, JsonObject pld){
    fixUp();
    if (!hd.isNull()) hd.set(jhd);
    if (!pld.isNull()) pld.set(jpld);
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
