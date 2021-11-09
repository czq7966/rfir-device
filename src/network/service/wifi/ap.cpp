#include "ap.h"

network::module::wifi::AP* network::service::wifi::AP::ap = 0;

void network::service::wifi::AP::Start(module::wifi::AP::Params p) {
    if (!ap)
        ap = new network::module::wifi::AP();
    ap->start(p);
}


void network::service::wifi::AP::Loop() {
    if (ap)
        ap->loop();
}