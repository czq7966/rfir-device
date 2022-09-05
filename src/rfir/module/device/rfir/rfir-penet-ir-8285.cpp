#include "rfir-penet-ir-8285.h"

void rfir::module::device::rfir::RFIRPenetIR8285::init() {
    RFIRPenet::init();
    sniffer->params.pin = 14;
    sender->params.pin = 4;
}