#include "sender.h"
#include "../serial/sender.h"
#include "../../module/cmds/cmd.h"
#include "../../module/ttl/sender.h"

rfir::module::ttl::Sender* rfir::service::ttl::Sender::ttlsender = new rfir::module::ttl::Sender();

void rfir::service::ttl::Sender::init(rfir::module::ttl::Sender::Params params) {
    ttlsender->init(params);
}



int rfir::service::ttl::Sender::send() {


}