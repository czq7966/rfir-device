#include "util.h"

uint64_t  rfir::util::Util::reverseBits(uint64_t input, uint16_t nbits) {
  if (nbits <= 1) return input;  // Reversing <= 1 bits makes no change at all.
  // Cap the nr. of bits to rotate to the max nr. of bits in the input.
  nbits = std::min(nbits, (uint16_t)(sizeof(input) * 8));
  uint64_t output = 0;
  for (uint16_t i = 0; i < nbits; i++) {
    output <<= 1;
    output |= (input & 1);
    input >>= 1;
  }
  // Merge any remaining unreversed bits back to the top of the reversed bits.
  return (input << nbits) | output;   
}

std::string rfir::util::Util::GetChipId(std::string prefix) {
#ifdef CHIP_ID_PREFIX
  prefix = CHIP_ID_PREFIX;
#endif  
#ifdef ESP8266
  uint32_t chipId = ESP.getChipId();
  return prefix + std::string(String(chipId).c_str());
#else		//ESP32
  uint32_t chipId = (uint32_t)ESP.getEfuseMac();
  return prefix + std::string(String(chipId, HEX).c_str());
#endif



  
}


std::string rfir::util::Util::BitsToString(uint8_t bytes[], uint16_t nbits) {
  String bitStr;
  for (size_t i = 0; i < nbits; i++)
  {
    uint8_t b = bytes[i / 8];
    uint16_t j = i % 8;
    b >>= j;
    bitStr = String(b & 1 ? 1: 0) + bitStr;      
  }
  return std::string(bitStr.c_str());
}

std::string rfir::util::Util::BytesToString(uint8_t bytes[], uint16_t nbytes) {
  std::string byteStr;
  for (size_t i = 0; i < nbytes; i++)
  {
    uint8_t b = bytes[i];
    if (i == 0)
      byteStr = BitsToString(bytes + i, 8);
    else
      byteStr = BitsToString(bytes + i, 8) + " " + byteStr;
  }
  return byteStr;
}
