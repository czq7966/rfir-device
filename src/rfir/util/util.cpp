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
#ifdef CHIP_ID_USE_DEC  
  String id = String(chipId);
#else
  String id = String(BytesToHexString((uint8_t*)(&chipId), sizeof(chipId), true).c_str());
  id.replace(" ", "");
#endif  
  return prefix + std::string(id.c_str());
#else		//ESP32
  uint32_t chipId = (uint32_t)ESP.getEfuseMac();
#ifdef CHIP_ID_USE_DEC  
  return prefix + std::string(String(chipId).c_str());
#else
  return prefix + std::string(String(chipId, HEX).c_str());
#endif

#endif
  
}

std::string rfir::util::Util::GetMacAddress() {
  uint8_t mac[6] = {};
  WiFi.macAddress(mac);
  return BytesToHexString(mac, 6, false, '-');
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
      byteStr = byteStr + " " + BitsToString(bytes + i, 8);
  }
  return byteStr;
}


std::string  rfir::util::Util::BytesToHexString(uint8_t bytes[], uint16_t nbytes, bool revert, char separater) {
    String result;
    int j = revert ? nbytes - 1 : 0;
    while (revert && j >=0 || !revert &&  j < nbytes)
    {
        char c[3];
        uint8_t b = bytes[j];  
        itoa(b, c, 16);
        String hex = (strlen(c) == 1) ? ("0" + String(c)) :  String(c);

        if (result.length() > 0)
            result = result + (separater + hex);
        else
            result = result + hex;     

        if (revert) j--; else j++;
    }
    return std::string(result.c_str()); 
}


int  rfir::util::Util::StringToBits(const char* data, int nbits, uint64_t& result) {
  for (size_t i = 0; i < nbits; i++)
  {
    data[i] == '0' ? result <<= 1 : result = (result << 1) | 1;    
  }
  
  return  nbits;  
}

int  rfir::util::Util::StringToBytes(const char* data, int nbits, uint8_t* bytes) {
  int offset = 0;
  for (size_t i = 0; i < nbits / 8; i++)
  {
    uint64_t byte = 0;
    offset += StringToBits(data + offset, 8, byte);
    bytes[i] = (uint8_t)byte; 
  }

  if (nbits % 8) {
      uint64_t byte = 0;
      offset += StringToBits(data + offset, nbits % 8, byte);
      bytes[offset / 8] = (uint8_t)byte; 
  }

  return offset;
}

int  rfir::util::Util::StringToBytes(std::string str, uint8_t* bytes) {
  String data(str.c_str());
  int idx = data.indexOf(",");
  
  if (idx > 0) {
    data = data.substring(0, idx);
  }

  int nbits = 0;
  data.replace(" ", "");
  
  int count = data.length();
  if (count > 0) {
    if (count > 2 && data[0] == '0' && data[1] == 'x') {
      for (size_t i = 2; i < count; i += 2)
      {
        char* str;
        String p = data.substring(i, i + 2);
        int len = 4 * p.length();   
       
        uint8_t byte = (uint8_t)strtol(p.c_str(), &str, 16);  
        bytes[(i - 2) / 2] = byte;
        nbits += len;
      }
    } else {
      nbits = StringToBytes(data.c_str(), count, bytes);
    }
  }

  return nbits;
}
