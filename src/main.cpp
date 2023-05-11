#include "service/device.h"
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
#include "rfir/ttl/sniffer.h"
#include "rfir/ttl/sender.h"
#include "service/networking.h"
#include "service/cmds.h"
#include "service/intranet.h"

//LED 获取闪灯
void* onGLedStart(void* arg , void* p) {
    
    //GWifiClient
    if (p == &GWifiClient) {
        return &WIFI_CONNECT_JLED;
    }

    //GAP
    if (p == &GWifiAP) {
        return &(AP_CONFIG_JLED);
    }
    
    //GMqttClient
    if (p == &GMqttClient) {
        return &MQTT_CONNECT_JLED;
    }

    return 0;
}


char config_filename[] = "/config.bin";
void setup() {
    int   pub_buffer_size = MQTT_BUFFER_SIZE;
    char* pub_buffer = (char*)malloc(pub_buffer_size);

    //GConfig
    GConfig.params.buf = (char*)pub_buffer;
    GConfig.params.bufsize = pub_buffer_size;
    GConfig.params.filename = config_filename;

    GConfig.init();    
    GConfig.fixUp();
    GConfig.syncRebootCount();
    GConfig.setRebootStartType(0, true);

    //GLed
    GLed.events.onStart.add((int32_t)0, onGLedStart);

    //Serial
    Serial.begin(GConfig.getSerialBaud(), (SerialConfig)GConfig.getSerialConfig());
    GDebuger.enabled =  GRegTable.tables.get(GRegTable.keys.serial_debug);
    GDebuger.print(F("begin chid id: "));
    GDebuger.print(GRegTable.values.dev_id);
    GDebuger.print(F(" , mac: "));
    GDebuger.println(GRegTable.values.dev_mac);

    //WIFI
    GWifiClient.addSsids(GRegTable.values.wifi_ssid, GRegTable.values.wifi_pass);
    GWifiClient.addSsids(GRegTable.values.wifi_ssid_dev, GRegTable.values.wifi_pass_dev);
    GWifiClient.params.timeout = GRegTable.tables.get(GRegTable.keys.wifi_reset_timeout) * 1000;

    //AP
    GWifiAP.params.apSsid = GRegTable.values.ap_ssid;
    GWifiAP.params.apPass = GRegTable.values.ap_pass;
    GWifiAP.params.start_wifi_timeout = GRegTable.tables.get(GRegTable.keys.ap_start_timeout) * 1000;


    //G MQTT
    int mqtt_msg_buffer_size = pub_buffer_size;
    char*  mqtt_msg_buffer = new char[mqtt_msg_buffer_size];
    GMqttClient.params.enable = !GRegTable.tables.get(GRegTable.keys.mqtt_disable);
    GMqttClient.params.msgbuf = mqtt_msg_buffer;
    GMqttClient.params.msgbufsize = mqtt_msg_buffer_size;
    GMqttClient.params.clean_session = GRegTable.tables.get(GRegTable.keys.mqtt_clean_session);
    GMqttClient.params.id = GRegTable.values.dev_id;
    GMqttClient.params.ip = GRegTable.values.mqtt_ip;
    GMqttClient.params.keepalive = GRegTable.tables.get(GRegTable.keys.mqtt_keepalive) * 1000;
    GMqttClient.params.pass = GRegTable.values.mqtt_pass;
    GMqttClient.params.port = GRegTable.tables.get(GRegTable.keys.mqtt_port);
    GMqttClient.params.timeout = GRegTable.tables.get(GRegTable.keys.mqtt_reset_timeout) * 1000;
    GMqttClient.params.user = GRegTable.values.mqtt_user;  

    //L MQTT    
    LMqttClient.params.msgbuf = mqtt_msg_buffer;
    LMqttClient.params.msgbufsize = mqtt_msg_buffer_size;
    LMqttClient.params.clean_session = GRegTable.tables.get(GRegTable.keys.mqtt_clean_session);
    LMqttClient.params.id = GRegTable.values.dev_id;    
    LMqttClient.params.keepalive = GRegTable.tables.get(GRegTable.keys.mqtt_keepalive) * 1000;    
    LMqttClient.params.timeout = GRegTable.tables.get(GRegTable.keys.mqtt_reset_timeout) * 1000;
    LMqttClient.params.enable = GRegTable.tables.get(GRegTable.keys.intranet_mqtt_enable);
    LMqttClient.params.ip = GRegTable.values.intranet_mqtt_ip;
    LMqttClient.params.port = GRegTable.tables.get(GRegTable.keys.intranet_mqtt_port);
    LMqttClient.params.user = strcmp(GRegTable.values.intranet_mqtt_user, "") == 0 ? GRegTable.values.mqtt_user : GRegTable.values.intranet_mqtt_user; 
    LMqttClient.params.pass = strcmp(GRegTable.values.intranet_mqtt_user, "") == 0 ? GRegTable.values.mqtt_pass : GRegTable.values.intranet_mqtt_pass; 

    //GSendCmd
    GSendCmd.params.buf = pub_buffer;
    GSendCmd.params.bufsize = pub_buffer_size;

    //GRecvCmd
    GRecvCmd.params.buf = mqtt_msg_buffer;
    GRecvCmd.params.bufsize = mqtt_msg_buffer_size;

    //OTA
    GOTAUpdater.params.id = GRegTable.values.dev_id;
    GOTAUpdater.params.interval = GRegTable.tables.get(GRegTable.keys.ota_update_interval) * 60 * 1000;
    GOTAUpdater.params.url = GRegTable.values.ota_update_url;
    GOTAUpdater.params.version = GRegTable.tables.get(GRegTable.keys.ota_version);
    GOTAUpdater.params.enabled = !GRegTable.tables.get(GRegTable.keys.ota_disable);
    GOTAUpdater.params.reboot = GRegTable.tables.get(GRegTable.keys.ota_reboot_after_check);

    //Sniffer
    GTTLSniffer.params.enabled = GRegTable.tables.get(GRegTable.keys.rfir_sniff_enable);
    GTTLSniffer.params.pin = GRegTable.tables.get(GRegTable.keys.rfir_sniff_pin);
    GTTLSniffer.params.inverted = GRegTable.tables.get(GRegTable.keys.rfir_sniff_inverted);
    GTTLSniffer.params.minCount = GRegTable.tables.get(GRegTable.keys.rfir_sniff_minCount);
    GTTLSniffer.params.maxCount = GRegTable.tables.get(GRegTable.keys.rfir_sniff_maxCount);
    GTTLSniffer.params.minDelta = GRegTable.tables.get(GRegTable.keys.rfir_sniff_minDelta);
    GTTLSniffer.params.maxDelta = GRegTable.tables.get(GRegTable.keys.rfir_sniff_maxDelta);

    //Sender
    GTTLSender.params.enabled = GRegTable.get(GRegTable.keys.rfir_send_enable);
    GTTLSender.params.pin = GRegTable.get(GRegTable.keys.rfir_send_pin);
    GTTLSender.params.inverted = GRegTable.get(GRegTable.keys.rfir_send_inverted);
    GTTLSender.params.modulation = GRegTable.get(GRegTable.keys.rfir_send_modulation);
    GTTLSender.params.repeat = GRegTable.get(GRegTable.keys.rfir_send_repeat);
    GTTLSender.params.frequency = GRegTable.get(GRegTable.keys.rfir_send_frequency);
    GTTLSender.params.dutycycle = GRegTable.get(GRegTable.keys.rfir_send_dutycycle);

    //GIntranet
    GIntranet.params.buf = mqtt_msg_buffer;
    GIntranet.params.bufsize = mqtt_msg_buffer_size;
       
    //WIFI
    GWifiClient.start();

    //G MQTT
    GMqttClient.start();

    //L MQTT
    LMqttClient.start();


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
    GDevice->params.bufsize = GRegTable.tables.get(GRegTable.keys.serial_read_bufsize);
    GDevice->params.buf = (char*)malloc(GDevice->params.bufsize);
    GDevice->start();

    //OTA
    GOTAUpdater.start();

    //Sniffer
    GTTLSniffer.start(true);

    //内网组网
    GIntranet.start(true);

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

    //射频采集
    GTTLSniffer.loop();

    //内网组网
    GIntranet.loop();
}


