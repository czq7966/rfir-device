#ifndef __RFIR_MODULE_DEVICE_SPEAKER_CL_CXY_SANSUI_P300_H__
#define __RFIR_MODULE_DEVICE_SPEAKER_CL_CXY_SANSUI_P300_H__


#include "sansui.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace speaker {
                class CL_CXY_SANSUI_P300: public SANSUI {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //
