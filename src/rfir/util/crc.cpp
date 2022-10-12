#include "crc.h"
#include "rfir/util/util.h"


uint16_t rfir::util::Crc::CRC16_Tab[] = {
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

uint16_t rfir::util::Crc::Calc_Crc16(uint8_t crcbuf, uint16_t crc_16) {
    uint8_t temp = ((uint8_t)(crc_16&0x000F))^(crcbuf&0x0F);
	crc_16 >>= 4;
	crc_16 ^= CRC16_Tab[temp];
	temp = ((uint8_t)(crc_16&0x000F))^(crcbuf>>4);
	crc_16 >>= 4;
	crc_16 ^= CRC16_Tab[temp];
    return crc_16;
};

uint16_t rfir::util::Crc::Get_CRC16(uint8_t *pBuff, uint16_t len) {
    uint8_t i;
    uint16_t crc16 = 0xFFFF;
   
    for (i=0;i<len;i++)
    {
        crc16 = Calc_Crc16(*pBuff++,crc16);
    }

    return crc16;
}

uint16_t rfir::util::Crc::Get_CRC16(uint8_t *pBuff, uint16_t len, uint8_t& crc16_low, uint8_t& crc16_high) {
    auto crc16 = Get_CRC16(pBuff, len);
    crc16_low = ((uint8_t*)&crc16)[0];
    crc16_high = ((uint8_t*)&crc16)[1];    
    return crc16;
}

std::string rfir::util::Crc::Get_CRC16_Str(std::string hexStr) {
    std::string result = hexStr;
    uint8_t bytes[hexStr.length()];
    int nbytes = rfir::util::Util::StringToBytes(hexStr, bytes) / 8;
    if (nbytes > 0) {
        auto crc16 = rfir::util::Crc::Get_CRC16(bytes, nbytes);
        result = hexStr + " " + rfir::util::Util::BytesToHexString((uint8_t*)&crc16, 2);
    }

    return result;
};