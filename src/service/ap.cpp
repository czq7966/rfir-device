#include "ap.h"
#include "cmds/cmd/reg-table.h"
#include "network/module/wifi/ap.h"
#include "network/module/wifi/client.h"
#include "config.h"
#include "rfir/util/serial.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"

void service::AP::init(){
    if (GRegTable.tables.get(GRegTable.keys.ap_disable) == 0) {
        GWifiAP.stop();

        GWifiAP.events.configSaved.add(this, [this](void* arg, void* p)->void*{this->onAPConfigSaved(arg, p); return 0;}, this);
        GWifiAP.events.applyDefault.add(this, [this](void* arg, void* p)->void*{this->onAPApplyDefault(arg, p); return 0;}, this);
    }
};
void service::AP::uninit(){
    GButton.events.onLongPressed.remove(this);
};

void service::AP::start(){
    init();
};

void service::AP::stop(){
    uninit();
};

void service::AP::resetConfig(){
    GConfig.resetConfig(true);
};


void* service::AP::onAPConfigSaved(void* arg, void* p){
    GDebuger.printf("GlobalConfig::onAPConfigSaved: %s \n", GWifiAP.wifiSsid);

    auto ap = &GWifiAP;

    if (strlen(ap->wifiSsid) > 0) {
        strcpy(GRegTable.values.wifi_ssid, ap->wifiSsid);
        strcpy(GRegTable.values.wifi_pass, ap->wifiPass);
    }

    //Serial
    GRegTable.tables.add(GRegTable.keys.serial_debug, strcmp(ap->serialDebug, "selected") == 0);
    GConfig.setSerialBaud(atoi(ap->serialBand));
    
    if (strlen(ap->serialConfig) > 0) {
        GSerialConfigs.setSerialConfig(ap->serialConfig);
        GRegTable.tables.add(GRegTable.keys.serial_data, GSerialConfigs.getSerialData());
        GRegTable.tables.add(GRegTable.keys.serial_stop, GSerialConfigs.getSerialStop());
        GRegTable.tables.add(GRegTable.keys.serial_sum, GSerialConfigs.getSerialSum());
    }

    GConfig.save();

    GEventTimer.delay(1000, [this](void*, void*)->void*{
        rfir::util::Util::Reset();
        return 0;
    });    

    return 0;
};

void* service::AP::onAPApplyDefault(void* arg, void* p){
    GDebuger.println(F("GlobalConfig::onAPApplyDefault "));

    auto ap = &GWifiAP;
    if (GWifiClient.params.ssid.size() > 0){
        strcpy(ap->wifiSsid, GWifiClient.params.ssid[0] );
        strcpy(ap->wifiPass, GWifiClient.params.pass[0]);
    }

    itoa(GConfig.getSerialBaud(), ap->serialBand, 10);
    const char* config;
    if (GSerialConfigs.configs.get(GConfig.getSerialConfig(), config)) {
        strcpy(ap->serialConfig, config);
    }    
    
    strcpy(ap->serialDebug , GRegTable.tables.get(GRegTable.keys.serial_debug) ? "selected" : "");

    return 0;
};
