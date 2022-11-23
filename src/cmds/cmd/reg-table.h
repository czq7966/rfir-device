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

                const uint16_t intranet_mqtt_ip =	60040;
                const uint16_t intranet_mqtt_user =	60041;
                const uint16_t intranet_mqtt_pass =	60042;
                const uint16_t intranet_ip =	    60043;

                //二进制
                const uint16_t penet_data =         60500;
                const uint16_t rfir_sniff_data =	60501;
                const uint16_t rfir_send_data = 	60502;
                const uint16_t report_penet_data = 	60503;
                const uint16_t report_reg_list = 	60504;


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

                const uint16_t gpio_rw_pin =         61035;
                const uint16_t gpio_rw_value =       61036;

                const uint16_t intranet_mqtt_enable =61040;
                const uint16_t intranet_mqtt_port =	 61041;
                const uint16_t intranet_enable =     61042;
                const uint16_t intranet_port =	     61043;
                const uint16_t intranet_from_id =	 61044;
                const uint16_t intranet_to_id =	     61045;
                    
                const uint16_t rfir_sniff_enable =	 61050;
                const uint16_t rfir_sniff_pin =	     61051;
                const uint16_t rfir_sniff_inverted = 61052;
                const uint16_t rfir_sniff_minCount = 61053;
                const uint16_t rfir_sniff_maxCount = 61054;
                const uint16_t rfir_sniff_minDelta = 61055;
                const uint16_t rfir_sniff_maxDelta = 61056;
                    
                const uint16_t rfir_send_enable =	 61060;
                const uint16_t rfir_send_pin =	     61061;
                const uint16_t rfir_send_inverted =	 61062;
                const uint16_t rfir_send_modulation =61063;
                const uint16_t rfir_send_repeat =	 61064;
                const uint16_t rfir_send_frequency = 61065;
                const uint16_t rfir_send_dutycycle = 61066;                

                //Report penet
                const uint16_t report_penet_enable = 61070; 
                const uint16_t report_penet_timeout= 61071; 
                
                //Report reg
                const uint16_t report_reg_enable   = 61080; 
                const uint16_t report_reg_timeout  = 61081; 

            };            
            struct Values
            {
                //字符串
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
                char intranet_mqtt_ip[65];
                char intranet_mqtt_user[33];
                char intranet_mqtt_pass[33];
                char intranet_ip[33];
                char intranet_from_id[33];
                char intranet_to_id[33];

                //二进制
                std::vector<uint8_t> report_penet_data;
                std::vector<uint8_t> report_reg_list;
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