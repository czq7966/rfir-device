#ifndef CMDS_CMD_REG_TABLE_H
#define CMDS_CMD_REG_TABLE_H

#include "rfir/util/reg-table.h"

namespace Cmds {
    namespace cmd {
        class RegTable: public rfir::util::RegTable {
        public:
        
            // struct Keys
            // {
            //     uint16_t cfg_version = 0;
            //     uint16_t dev_vender = 0;
            //     uint16_t dev_model = 0;
            //     uint16_t dev_id = 0;
            //     uint16_t dev_mac = 0;
            //     uint16_t dev_online = 0;
                
            //     uint16_t pin_led = 0;
            //     uint16_t pin_button = 0;
            //     uint16_t pin_reset = 0;
                
            //     uint16_t serial_debug = 0;
            //     uint16_t serial_baud = 0;
            //     uint16_t serial_data = 0;
            //     uint16_t serial_stop = 0;
            //     uint16_t serial_sum= 0;
            //     uint16_t serial_stream = 0;

            //     uint16_t ota_disable = 0;
            //     uint16_t ota_version = 0;
            //     uint16_t ota_update_url = 0;
            //     uint16_t ota_update_interval = 0;

            //     uint16_t wifi_disable = 0;
            //     uint16_t wifi_ssid_dev = 0;
            //     uint16_t wifi_pass_dev = 0;
            //     uint16_t wifi_ssid = 0;
            //     uint16_t wifi_pass = 0;
            //     uint16_t wifi_reset_timeout = 0;
            //     uint16_t wifi_rssi = 0;

            //     uint16_t ap_disable = 0;
            //     uint16_t ap_start_timeout = 0;
            //     uint16_t ap_ssid = 0;
            //     uint16_t ap_pass = 0;

            //     uint16_t mqtt_disable = 0;
            //     uint16_t mqtt_ip = 0;
            //     uint16_t mqtt_port = 0;
            //     uint16_t mqtt_user = 0;
            //     uint16_t mqtt_pass = 0;
            //     uint16_t mqtt_keepalive = 0;
            //     uint16_t mqtt_clean_session = 0;
            //     uint16_t mqtt_reset_timeout = 0;
            //     uint16_t mqtt_buffer_size = 0;
            //     uint16_t mqtt_sub_topic = 0;
            //     uint16_t mqtt_pub_topic = 0;

            //     uint16_t net_report_timout = 0;
            //     uint16_t net_handshake_timout = 0;
            // };
            
            struct Keys {
                const uint16_t dev_vender =         60000;
                const uint16_t dev_model =          60001;
                const uint16_t dev_id =             60002;
                const uint16_t dev_mac =            60003;
                const uint16_t wifi_ssid =          60004;
                const uint16_t wifi_pass =          60005;
                const uint16_t ap_ssid =            60006;
                const uint16_t ap_pass =            60007;
                const uint16_t ota_update_url =     60008;
                const uint16_t mqtt_sub_topic =     60009;
                const uint16_t mqtt_pub_topic =     60010;
                const uint16_t mqtt_ip =            60011;
                const uint16_t mqtt_user =          60012;
                const uint16_t mqtt_pass =          60013;
                const uint16_t wifi_ssid_dev =      60014;
                const uint16_t wifi_pass_dev =      60015;

                const uint16_t dev_online =         61000;
                const uint16_t cfg_version =        61001;
                const uint16_t wifi_rssi =          61002;
                const uint16_t pin_led =            61003;
                const uint16_t pin_button =         61004;
                const uint16_t pin_reset =          61005;

                const uint16_t serial_debug =       61006;
                const uint16_t serial_baud =        61007;
                const uint16_t serial_data =        61008;
                const uint16_t serial_stop =        61009;
                const uint16_t serial_sum =         61010;
                const uint16_t serial_stream =      61011;

                const uint16_t ota_disable =        61012;
                const uint16_t ota_version =        61013;
                const uint16_t ota_update_interval =61014;

                const uint16_t wifi_disable =       61015;
                const uint16_t wifi_reset_timeout = 61016;

                const uint16_t ap_disable =         61017;
                const uint16_t ap_start_timeout =   61018;

                const uint16_t mqtt_disable =       61019;
                const uint16_t mqtt_port =          61020;
                const uint16_t mqtt_keepalive =     61021;
                const uint16_t mqtt_clean_session = 61022;
                const uint16_t mqtt_reset_timeout = 61023;
                const uint16_t mqtt_buffer_size =   61024;

                const uint16_t net_report_timeout =  61025;
                const uint16_t net_handshake_timeout=61026;
            };            
            struct Values
            {
                char dev_vender[32];
                char dev_model[32];
                char dev_id[32];
                char dev_mac[32];
                char wifi_ssid[64];
                char wifi_pass[64];
                char ap_ssid[32];
                char ap_pass[32];
                char ota_update_url[160];
                char mqtt_sub_topic[64];
                char mqtt_pub_topic[64];
                char mqtt_ip[64];
                char mqtt_user[32];
                char mqtt_pass[32];
                char wifi_ssid_dev[32];
                char wifi_pass_dev[32];
            };
            
        public:
            Keys keys;
            Values values;
            RegTable();

        };
    }
}
                
extern Cmds::cmd::RegTable GRegTable;

#endif                