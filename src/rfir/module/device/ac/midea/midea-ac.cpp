#include "midea-ac.h"
#include "rfir/util/util.h"
#include "rfir/rfir.h"



rfir::module::device::ac::MideaAC::MideaAC(){
    protocol_close.remote_state = P_Close;
    setPower(0);
    setMode(P_Mode_Cool);
    setTemp(25);
    setFan(P_Fan_Auto);
    fixUp();
};

std::string rfir::module::device::ac::MideaAC::toString() {
    return "";
}

std::string rfir::module::device::ac::MideaAC::toBitString() {
    int count;
    auto raw = getRaw(count);
    return rfir::util::Util::BytesToString(raw, count);
}

std::string rfir::module::device::ac::MideaAC::toHexString() {
    int count;
    auto raw = getRaw(count);
    return rfir::util::Util::BytesToHexString(raw, count);
}

uint16_t* rfir::module::device::ac::MideaAC::getEncodeRaw(int& count) {
    uint16_t offset = 0;

    //Header
    this->encodeRaw[offset++] = KHdrMark;
    this->encodeRaw[offset++] = KHdrSpace;

    auto raw = getRaw(count);
    
    if (!getPower())
        raw = (uint8_t*)&protocol_close.remote_state;    
    auto rfir = rfir::GetRfir();
    bool MSBfirst = rfir->encoder->getEncodeParams()->params[0].MSBfirst;
    
    //Data
    for(uint8_t i = 0; i < count; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            int bit;
            bit = MSBfirst ? (byte >> (7 - j) & 0x01) : (byte >> j & 0x01);

            uint16_t mark = 0, space = 0;

            if (bit == 0) {
                mark = KZeroMark;
                space = KZeroSpace;
            } else {
                mark = KOneMark;
                space = KOneSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //Footer
    this->encodeRaw[offset++] = KFtrMark;
    this->encodeRaw[offset++] = KFtrSpace;

    count = KEncodeRawLength;
    return this->encodeRaw;    
}

// std::string rfir::module::device::ac::MideaAC::getEncodeString() {
//     int count;
//     auto raw = getEncodeRaw(count);
//     String result;
//     for(auto i = 0; i < count; i++) {
//         result = result + String(*(raw + i)) + ",";
//     }
//     return std::string(result.c_str());
// }


uint8_t rfir::module::device::ac::MideaAC::getPower(){
    return protocol.Power;
};
void rfir::module::device::ac::MideaAC::setPower(uint8_t v){
    protocol.Power = v;
};
uint8_t rfir::module::device::ac::MideaAC::getMode(){
    return protocol.Mode;
};
void rfir::module::device::ac::MideaAC::setMode(uint8_t v){
    protocol.Mode = v;
};
uint8_t rfir::module::device::ac::MideaAC::getTemp(){
    auto temp = protocol.Temp;
    int len = std::end(P_Temp) - std::begin(P_Temp);
    for (size_t i = 0; i < len; i++)
    {
        if (P_Temp[i] == temp) {
            temp = i + P_Temp_Min;
            break;
        }        
    }
    
    return temp;
};
void rfir::module::device::ac::MideaAC::setTemp(uint8_t v){
    auto temp = std::max(v, P_Temp_Min);
    temp = std::min(temp, P_Temp_Max);
    temp = temp - P_Temp_Min;
    temp = P_Temp[temp];
    protocol.Temp = temp;
};

uint8_t rfir::module::device::ac::MideaAC::getFan(){
    return protocol.Fan;
};
void rfir::module::device::ac::MideaAC::setFan(uint8_t v){
    protocol.Fan = v;
};

void rfir::module::device::ac::MideaAC::fixUp(){
    setPower(getPower());
    protocol.BT0 = P_BT0;
    protocol.BT1 = ~P_BT0;
    protocol.BT2_1 = P_BT2_1;
    protocol.BT2_2 = P_BT2_2;    
    protocol.BT3 = ~(((uint8_t*)&protocol.remote_state)[2]);
    protocol.BT4_1 = P_BT4_1;
    protocol.BT5 = ~(((uint8_t*)&protocol.remote_state)[4]);    
};


bool rfir::module::device::ac::MideaAC::setRaw(uint8_t* raw){
    auto temp = protocol.remote_state;
    memcpy(&protocol.remote_state, raw, KBytesLength);
    if (!getPower()) {
        protocol.remote_state = temp;
        setPower(0);
    }
    fixUp();     
    return 1;
};
uint8_t* rfir::module::device::ac::MideaAC::getRaw(int& count){
    fixUp();
    count = KBytesLength;
    return (uint8_t*)&protocol.remote_state;
};    