#include "service/config.h"
#include <Arduino.h>
#include "IotWebConf.h"
#include "network/module/mqtt/async-client.h"
#include "network/module/wifi/client.h"
#include "network/module/wifi/ap.h"
#include "network/module/ota/updater.h"
#include "cmds/cmd/cmd.h"
#include "cmds/cmd/reg-table.h"
#include "rfir/util/event-timer.h"
#include "service/networking.h"
#include "service/cmds.h"


// char* mqtt_recv_buffer = new char[GRegTable.];

char config_filename[] = "/config.bin";
char* pub_buffer = (char*)malloc(1024 * 4);
void setup() {
    int  pub_buffer_size = MQTT_BUFFER_SIZE;
    GConfig.params.buf = (char*)pub_buffer;
    GConfig.params.bufsize = pub_buffer_size;
    GConfig.params.filename = config_filename;

    GConfig.init();    
    GConfig.fixUp();

    Serial.begin(GConfig.getSerialBaud(), (SerialConfig)GConfig.getSerialConfig());
    GDebuger.enabled =  GRegTable.tables.get(GRegTable.keys.serial_debug);

    GSendCmd.params.buf = pub_buffer;
    GSendCmd.params.bufsize = pub_buffer_size;
    

    //WIFI
    GWifiClient.addSsids(GRegTable.values.wifi_ssid, GRegTable.values.wifi_pass);
    GWifiClient.addSsids(GRegTable.values.wifi_ssid_dev, GRegTable.values.wifi_pass_dev);
    GWifiClient.params.timeout = GRegTable.tables.get(GRegTable.keys.wifi_reset_timeout) * 1000;
    GWifiClient.params.jled = &WIFI_CONNECT_JLED;

    //AP
    GWifiAP.params.apSsid = GRegTable.values.ap_ssid;
    GWifiAP.params.apPass = GRegTable.values.ap_pass;
    GWifiAP.params.jled = &AP_CONFIG_JLED;
    GWifiAP.params.start_wifi_timeout = GRegTable.tables.get(GRegTable.keys.ap_start_timeout) * 1000;


    //MQTT
    int mqtt_msg_buffer_size = pub_buffer_size;
    char*  mqtt_msg_buffer = new char[mqtt_msg_buffer_size];
    GMqttClient.params.msgbuf = mqtt_msg_buffer;
    GMqttClient.params.msgbufsize = mqtt_msg_buffer_size;
    GMqttClient.params.clean_session = GRegTable.tables.get(GRegTable.keys.mqtt_clean_session);
    GMqttClient.params.id = GRegTable.values.dev_id;
    GMqttClient.params.ip = GRegTable.values.mqtt_ip;
    GMqttClient.params.jled = &MQTT_CONNECT_JLED;
    GMqttClient.params.keepalive = GRegTable.tables.get(GRegTable.keys.mqtt_keepalive) * 1000;
    GMqttClient.params.pass = GRegTable.values.mqtt_pass;
    GMqttClient.params.port = GRegTable.tables.get(GRegTable.keys.mqtt_port);
    GMqttClient.params.timeout = GRegTable.tables.get(GRegTable.keys.mqtt_reset_timeout) * 1000;
    GMqttClient.params.user = GRegTable.values.mqtt_user;  


    GOTAUpdater.params.id = GRegTable.values.dev_id;
    GOTAUpdater.params.interval = GRegTable.tables.get(GRegTable.keys.ota_update_interval) * 60 * 1000;
    GOTAUpdater.params.url = GRegTable.values.ota_update_url;
    GOTAUpdater.params.version = GRegTable.tables.get(GRegTable.keys.ota_version);
    GOTAUpdater.params.enabled = !GRegTable.tables.get(GRegTable.keys.ota_disable);

    
       
    //WIFI
    GWifiClient.start();

    //MQTT
    GMqttClient.start();

    //启动组网组件
    GNetworking.start();    

    //定时器
    GEventTimer.start();

    //按钮
    GButton.start(GRegTable.tables.get(GRegTable.keys.pin_button));

    //执点
    GAP.start();

    //指令
    GCmds.start();

    //设备
    GDevice->start();

    //OTA
    GOTAUpdater.start();

    //配置Ready
    GConfig.events.ready.emit(0);
    GDebuger.println(F("Ready"));

}



void loop() {
    //设备
    GDevice->loop();

    //指令
    GCmds.loop();

        //LED
    GLed.loop();

    //按钮
    GButton.loop();

    //定时器
    GEventTimer.loop();

    //热点
    GAP.loop();  
}


