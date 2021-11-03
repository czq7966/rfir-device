#ifndef __RFIR_MODULE_DEVICE_AC_KELVINATOR_H__
#define __RFIR_MODULE_DEVICE_AC_KELVINATOR_H__


#include "../../device.h"
#include "rfir/rfir.h"
#include "ir_kelvinator.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Kelvinator: public Device {
                public:
                    // Constants
                    const uint16_t kKelvinatorTick = 85;
                    const uint16_t kKelvinatorHdrMarkTicks = 106;
                    const uint16_t kKelvinatorHdrMark = kKelvinatorHdrMarkTicks * kKelvinatorTick;
                    const uint16_t kKelvinatorHdrSpaceTicks = 53;
                    const uint16_t kKelvinatorHdrSpace = kKelvinatorHdrSpaceTicks * kKelvinatorTick;
                    const uint16_t kKelvinatorBitMarkTicks = 7;
                    const uint16_t kKelvinatorBitMark = kKelvinatorBitMarkTicks * kKelvinatorTick;
                    const uint16_t kKelvinatorOneSpaceTicks = 20;
                    const uint16_t kKelvinatorOneSpace = kKelvinatorOneSpaceTicks * kKelvinatorTick;
                    const uint16_t kKelvinatorZeroSpaceTicks = 7;
                    const uint16_t kKelvinatorZeroSpace =
                        kKelvinatorZeroSpaceTicks * kKelvinatorTick;
                    const uint16_t kKelvinatorGapSpaceTicks = 235;
                    const uint16_t kKelvinatorGapSpace = kKelvinatorGapSpaceTicks * kKelvinatorTick;

                    const uint8_t kKelvinatorCmdFooter = 2;
                    const uint8_t kKelvinatorCmdFooterBits = 3;

                    const uint8_t kKelvinatorChecksumStart = 10;                
                protected:
                    IRKelvinatorAC* ac = 0;
                public:                     
                    Kelvinator();
                    ~Kelvinator();
                    
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;            
                    virtual void start(void *) override;
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual uint16_t* getEncodeRaw(int& count);         
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override; 
                };
            }
        }
    }
}



#endif //
