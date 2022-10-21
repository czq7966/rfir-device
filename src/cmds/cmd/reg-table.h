#ifndef CMDS_CMD_REG_TABLE_H
#define CMDS_CMD_REG_TABLE_H

#include "rfir/util/reg-table.h"

namespace cmds {
    namespace cmd {
        class RegTable: public rfir::util::RegTable {
        public:        
            struct Keys {
                //字符串
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

                //二进制
                const uint16_t penet_data =         60500;

                //数字
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
                const uint16_t net_handshake_count  =61027;

                const uint16_t dev_address =         61028;
                const uint16_t dev_offline_count =   61029;
                const uint16_t net_report_reason =   61030;

                const uint16_t serial_read_timeout = 61031;
                const uint16_t serial_read_bufsize = 61032;
                const uint16_t serial_half_rw_pin =  61033;
                const uint16_t serial_half_r_level = 61034;

            };            
            struct Values
            {
                char dev_vender[33];
                char dev_model[33];
                char dev_id[33];
                char dev_mac[33];
                char wifi_ssid[65];
                char wifi_pass[65];
                char ap_ssid[33];
                char ap_pass[33];
                char ota_update_url[161];
                char mqtt_sub_topic[65];
                char mqtt_pub_topic[65];
                char mqtt_ip[65];
                char mqtt_user[33];
                char mqtt_pass[33];
                char wifi_ssid_dev[33];
                char wifi_pass_dev[33];
            };
            
        public:
            Keys keys;
            Values values;
            RegTable();
        };
    }
}
                
extern cmds::cmd::RegTable GRegTable;

#endif                