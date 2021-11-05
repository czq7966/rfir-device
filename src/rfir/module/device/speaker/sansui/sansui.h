#ifndef __RFIR_MODULE_DEVICE_SPEAKER_SANSUI_H__
#define __RFIR_MODULE_DEVICE_SPEAKER_SANSUI_H__


#include "../../device.h"
// #include "../../ac/mcquay/cl-djroom-mcquay.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace speaker {
                class SANSUI: public Device {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //
