#include "config.h"
#include "rfir/util/file.h"
#include "rfir/util/util.h"
#include "rfir/util/serial.h"
#include "rfir/util/interrupt.h"


void service::Config::init(){
    strcpy(GRegTable.values.dev_vender, DEV_VENDOR);
    strcpy(GRegTable.values.dev_model, DEV_MODEL);
    rfir::util::Util::GetChipId(GRegTable.values.dev_id, CHIP_ID_PREFIX, CHIP_ID_USE_MAC);
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
    strcpy(GRegTable.values.intranet_mqtt_ip, INTRANET_MQTT_IP);
    strcpy(GRegTable.values.intranet_mqtt_user, INTRANET_MQTT_USER);
    strcpy(GRegTable.values.intranet_mqtt_pass, INTRANET_MQTT_PASS);
    strcpy(GRegTable.values.intranet_ip, INTRANET_IP);

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
    // GRegTable.tables.add(GRegTable.keys.dev_offline_count, 0);
    // GRegTable.tables.add(GRegTable.keys.net_report_reason, 0);


    GRegTable.tables.add(GRegTable.keys.serial_read_timeout, SERIAL_READ_TIMEOUT);
    GRegTable.tables.add(GRegTable.keys.serial_read_bufsize, SERIAL_READ_BUFSIZE);
    GRegTable.tables.add(GRegTable.keys.serial_half_rw_pin, SERIAL_HALF_RW_PIN);
    GRegTable.tables.add(GRegTable.keys.serial_half_r_level, SERIAL_HALF_R_LEVEL);

    // GRegTable.tables.add(GRegTable.keys.gpio_rw_pin, 0);
    // GRegTable.tables.add(GRegTable.keys.gpio_rw_value, 0);

    GRegTable.tables.add(GRegTable.keys.intranet_mqtt_enable, INTRANET_MQTT_ENABLE);
    GRegTable.tables.add(GRegTable.keys.intranet_mqtt_port, INTRANET_MQTT_PORT);
    GRegTable.tables.add(GRegTable.keys.intranet_enable, INTRANET_ENABLE);
    GRegTable.tables.add(GRegTable.keys.intranet_port, INTRANET_PORT);

    GRegTable.tables.add(GRegTable.keys.rfir_sniff_enable, RFIR_SNIFF_ENABLE);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_pin, RFIR_SNIFF_PIN);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_inverted, RFIR_SNIFF_INVERTED);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_minCount, RFIR_SNIFF_MINCOUNT);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_maxCount, RFIR_SNIFF_MAXCOUNT);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_minDelta, RFIR_SNIFF_MINDELTA);
    GRegTable.tables.add(GRegTable.keys.rfir_sniff_maxDelta, RFIR_SNIFF_MAXDELTA);

    GRegTable.tables.add(GRegTable.keys.rfir_send_enable, RFIR_SEND_ENABLE);
    GRegTable.tables.add(GRegTable.keys.rfir_send_pin, RFIR_SEND_PIN);
    GRegTable.tables.add(GRegTable.keys.rfir_send_inverted, RFIR_SEND_INVERTED);
    GRegTable.tables.add(GRegTable.keys.rfir_send_modulation, RFIR_SEND_MODULATION);
    GRegTable.tables.add(GRegTable.keys.rfir_send_repeat, RFIR_SEND_REPEAT);
    GRegTable.tables.add(GRegTable.keys.rfir_send_frequency, RFIR_SEND_FREQUENCY);
    GRegTable.tables.add(GRegTable.keys.rfir_send_dutycycle, RFIR_SEND_DUTYCYCLE);


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

    strcpy(GRegTable.values.ap_ssid, GRegTable.values.dev_id);

    this->load();

};

void service::Config::load() {
    rfir::util::File file(this->params.filename);
    auto count = file.read(this->params.buf, this->params.bufsize);
    if (count > 0) {
        this->saved.clear();
        GRegTable.load(this->params.buf, count, this->saved);
    }
    this->events.loaded.emit(this);    
};

void service::Config::save() {
    if (this->saved.getSize() == 0)
        this->resetConfig(false);
    else {
        std::list<int> ids;
        this->saved.getKeys(ids);
        this->save(ids);
    }
};   

void service::Config::save(std::list<int> ids) {
    int size = 0;
    if (GRegTable.encode(this->params.buf, size, ids)) {
        rfir::util::File file(this->params.filename);
        GInterrupt.stop();
        file.write(this->params.buf, size);
        GInterrupt.start();
        this->events.saved.emit(this);
    }
};

void service::Config::resetConfig(bool restart){
    GDebuger.println("service::Config::resetConfig");
    rfir::util::File::remove(this->params.filename);
    if (restart)
        rfir::util::Util::Reset(1000);
};

void service::Config::existSavedKey(int key){

};

void service::Config::addSavedKey(int key){

};

void service::Config::removeSavedKey(int key){

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