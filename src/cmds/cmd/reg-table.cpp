#include "reg-table.h"
#include "rfir/util/debuger.h"

cmds::cmd::RegTable::RegTable(){
    this->tables.add(this->keys.dev_vender, (int)this->values.dev_vender);
    this->tables.add(this->keys.dev_model, (int)this->values.dev_model);
    this->tables.add(this->keys.dev_id, (int)this->values.dev_id);
    this->tables.add(this->keys.dev_mac, (int)this->values.dev_mac);
    this->tables.add(this->keys.wifi_ssid, (int)this->values.wifi_ssid);
    this->tables.add(this->keys.wifi_pass, (int)this->values.wifi_pass);
    this->tables.add(this->keys.ap_ssid, (int)this->values.ap_ssid);
    this->tables.add(this->keys.ap_pass, (int)this->values.ap_pass);
    this->tables.add(this->keys.ota_update_url, (int)this->values.ota_update_url);
    this->tables.add(this->keys.mqtt_sub_topic, (int)this->values.mqtt_sub_topic);
    this->tables.add(this->keys.mqtt_pub_topic, (int)this->values.mqtt_pub_topic);
    this->tables.add(this->keys.mqtt_ip, (int)this->values.mqtt_ip);
    this->tables.add(this->keys.mqtt_user, (int)this->values.mqtt_user);
    this->tables.add(this->keys.mqtt_pass, (int)this->values.mqtt_pass);
    this->tables.add(this->keys.wifi_ssid_dev, (int)this->values.wifi_ssid_dev);
    this->tables.add(this->keys.wifi_pass_dev, (int)this->values.wifi_pass_dev);

    this->tables.add(this->keys.dev_online, 0);
    this->tables.add(this->keys.cfg_version, 0);
    this->tables.add(this->keys.wifi_rssi, 0);
    this->tables.add(this->keys.pin_led, 0);
    this->tables.add(this->keys.pin_button, 0);
    this->tables.add(this->keys.pin_reset, 0);

    this->tables.add(this->keys.serial_debug, 0);
    this->tables.add(this->keys.serial_baud, 0);
    this->tables.add(this->keys.serial_data, 0);
    this->tables.add(this->keys.serial_stop, 0);
    this->tables.add(this->keys.serial_sum, 0);
    this->tables.add(this->keys.serial_stream, 0);

    this->tables.add(this->keys.ota_disable, 0);
    this->tables.add(this->keys.ota_version, 0);
    this->tables.add(this->keys.ota_update_interval, 0);

    this->tables.add(this->keys.wifi_disable, 0);
    this->tables.add(this->keys.wifi_reset_timeout, 0);

    this->tables.add(this->keys.ap_disable, 0);
    this->tables.add(this->keys.ap_start_timeout, 0);

    this->tables.add(this->keys.mqtt_disable, 0);
    this->tables.add(this->keys.mqtt_port, 0);
    this->tables.add(this->keys.mqtt_keepalive, 0);
    this->tables.add(this->keys.mqtt_clean_session, 0);
    this->tables.add(this->keys.mqtt_reset_timeout, 0);
    this->tables.add(this->keys.mqtt_buffer_size, 0);

    this->tables.add(this->keys.net_report_timeout, 0);
    this->tables.add(this->keys.net_handshake_timeout, 0);   
    this->tables.add(this->keys.net_handshake_count, 0);  
    

    this->tables.add(this->keys.dev_address, 0);      
    this->tables.add(this->keys.dev_offline_count, 0); 

    this->tables.add(this->keys.net_report_reason, 0);   

    this->tables.add(this->keys.serial_read_timeout, 0);     

};

cmds::cmd::RegTable GRegTable;