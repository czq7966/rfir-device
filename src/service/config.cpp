#include "config.h"
#include "rfir/util/file.h"
#include "rfir/util/util.h"
#include "rfir/util/serial.h"

service::Config::Config(){
    // this->init();
};

void service::Config::init(){
    strcpy(GRegTable.values.dev_vender, DEV_VENDOR);
    strcpy(GRegTable.values.dev_model, DEV_MODEL);
    rfir::util::Util::GetChipId(GRegTable.values.dev_id, CHIP_ID_PREFIX);
    rfir::util::Util::GetMacAddress(GRegTable.values.dev_mac);
    strcpy(GRegTable.values.wifi_ssid, WIFI_SSID);
    strcpy(GRegTable.values.wifi_pass, WIFI_PASS);
    strcpy(GRegTable.values.ap_ssid, AP_SSID);
    strcpy(GRegTable.values.ap_pass, AP_PASS);
    strcpy(GRegTable.values.ota_update_url, OTA_UPDATE_URL);
    strcpy(GRegTable.values.mqtt_sub_topic, MQTT_SUB_TOPIC);
    strcpy(GRegTable.values.mqtt_pub_topic, MQTT_PUB_TOPIC);
    strcpy(GRegTable.values.mqtt_ip, MQTT_IP);
    strcpy(GRegTable.values.mqtt_user, MQTT_USER);
    strcpy(GRegTable.values.mqtt_pass, MQTT_PASS);
    strcpy(GRegTable.values.wifi_ssid_dev, WIFI_SSID_DEV);
    strcpy(GRegTable.values.wifi_pass_dev, WIFI_PASS_DEV);

    GRegTable.tables.add(GRegTable.keys.dev_online, 0);
    GRegTable.tables.add(GRegTable.keys.cfg_version, CFG_VERSION);
    GRegTable.tables.add(GRegTable.keys.wifi_rssi, 0);
    if (WiFi.isConnected())
        GRegTable.tables.add(GRegTable.keys.wifi_rssi, WiFi.RSSI());
    GRegTable.tables.add(GRegTable.keys.pin_led, PIN_LED);
    GRegTable.tables.add(GRegTable.keys.pin_button, PIN_BUTTON);
    GRegTable.tables.add(GRegTable.keys.pin_reset, PIN_RESET);

    GRegTable.tables.add(GRegTable.keys.serial_debug, SERIAL_DEBUG);
    GRegTable.tables.add(GRegTable.keys.serial_baud, SERIAL_BAUD);
    GRegTable.tables.add(GRegTable.keys.serial_data, SERIAL_DATA);
    GRegTable.tables.add(GRegTable.keys.serial_stop, SERIAL_STOP);
    GRegTable.tables.add(GRegTable.keys.serial_sum, SERIAL_SUM);
    GRegTable.tables.add(GRegTable.keys.serial_stream, SERIAL_STREAM);

    GRegTable.tables.add(GRegTable.keys.ota_disable, OTA_DISABLE);
    GRegTable.tables.add(GRegTable.keys.ota_version, OTA_VERSION);
    GRegTable.tables.add(GRegTable.keys.ota_update_interval, OTA_UPDATE_INTERVAL);

    GRegTable.tables.add(GRegTable.keys.wifi_disable, WIFI_DISABLE);
    GRegTable.tables.add(GRegTable.keys.wifi_reset_timeout, WIFI_RESET_TIMEOUT);

    GRegTable.tables.add(GRegTable.keys.ap_disable, AP_DISABLE);
    GRegTable.tables.add(GRegTable.keys.ap_start_timeout, AP_START_TIMEOUT);

    GRegTable.tables.add(GRegTable.keys.mqtt_disable, MQTT_DISABLE);
    GRegTable.tables.add(GRegTable.keys.mqtt_port, MQTT_PORT);
    GRegTable.tables.add(GRegTable.keys.mqtt_keepalive, MQTT_KEEPALIVE);
    GRegTable.tables.add(GRegTable.keys.mqtt_clean_session, MQTT_CLEAN_SESSION);
    GRegTable.tables.add(GRegTable.keys.mqtt_reset_timeout, MQTT_RESET_TIMEOUT);
    GRegTable.tables.add(GRegTable.keys.mqtt_buffer_size, MQTT_BUFFER_SIZE);

    GRegTable.tables.add(GRegTable.keys.net_report_timeout, NET_REPORT_TIMEOUT);
    GRegTable.tables.add(GRegTable.keys.net_handshake_timeout, NET_HANDSHAKE_TIMEOUT);

    GRegTable.tables.add(GRegTable.keys.dev_address, DEV_ADDRESS);


    //Button
    GButton.events.onLongPressed.once(this, [this](void*, void*)->void*{ this->resetConfig(); return 0;}, this, &this->keyTimeResetConfig);

};

void service::Config::fixUp(){
    strcpy(GRegTable.values.mqtt_sub_topic,"0/0/0/0/dev/");
    strcat(GRegTable.values.mqtt_sub_topic, GRegTable.values.dev_id);
    strcat(GRegTable.values.mqtt_sub_topic, "/0/0");

    strcpy(GRegTable.values.mqtt_pub_topic,"0/0/dev/");
    strcat(GRegTable.values.mqtt_pub_topic, GRegTable.values.dev_id);
    strcat(GRegTable.values.mqtt_pub_topic, "/0/0/0/0");

    char temp[32];
    rfir::util::Util::GetChipId(temp);
    strcpy(GRegTable.values.ap_ssid, "ndiot_");
    strcat(GRegTable.values.ap_ssid, temp);



};

void service::Config::load() {
    rfir::util::File file(this->params.filename);
    auto count = file.read(this->params.buf, this->params.bufsize);
    if (count > 0) {
        GRegTable.decode(this->params.buf, count);
        this->events.loaded.emit(this);
    }

};

void service::Config::save() {
    std::list<int> ids;
    ids.push_back(60000);
    ids.push_back(60001);
    save(ids);

};   

void service::Config::save(std::list<int> ids) {
    int size = 0;
    if (GRegTable.encode(this->params.buf, size, ids)) {
        GRegTable.decode(this->params.buf, size);
        // rfir::util::File file(this->params.filename);
        // file.write(this->params.buf, size);
        // this->events.saved.emit(this);
    }
};

void service::Config::resetConfig(bool restart){
    Serial.println("service::Config::resetConfig");
    delay(1000);
    rfir::util::File::remove(this->params.filename);
    if (restart)
        rfir::util::Util::Reset();
};

int service::Config::getSerialBaud(){
    return GRegTable.tables.get(GRegTable.keys.serial_baud) * 100;
};

void service::Config::setSerialBaud(int value){
    GRegTable.tables.add(GRegTable.keys.serial_baud, value / 100);
};

int service::Config::getSerialConfig(){
    auto data = GRegTable.tables.get(GRegTable.keys.serial_data);
    auto stop = GRegTable.tables.get(GRegTable.keys.serial_stop);
    auto sum = GRegTable.tables.get(GRegTable.keys.serial_sum);
    GSerialConfigs.setSerialData(data);
    GSerialConfigs.setSerialStop(stop);
    GSerialConfigs.setSerialSum(sum);

    return GSerialConfigs.getSerialConfig();
};



service::Config GConfig;
JLed  GJLed = JLed(PIN_LED);