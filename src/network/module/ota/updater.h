
#ifndef __NETWORK_MODULE_OTA_UPDATER_H__
#define __NETWORK_MODULE_OTA_UPDATER_H__

#ifdef ESP8266
#include <string>
#include <EOTAUpdate.h>

namespace network {
    namespace module {
        namespace ota {
            class Updater {
            private:
                EOTAUpdate* eOtaUpdater = 0;


            public:
                struct Params {
                    std::string url;
                    std::string id;
                    unsigned version;
                };            
            public:
                void start(Params p);
                void loop();

            };

        }
    }
}

#endif //ESP8266

#endif //
