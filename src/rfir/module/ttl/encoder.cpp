#include "encoder.h"
#include "config.h"
#include <sys/time.h>
#include "rfir/util/util.h"

rfir::module::ttl::Encoder::Encoder() {

}

rfir::module::ttl::Encoder::~Encoder() {  
  setEncodeParams(0);
  uninitEncodeResult();
}


void rfir::module::ttl::Encoder::initEncodeResult() {
  uninitEncodeResult();
  this->encodeResult.result = new uint16_t[1024*2];
  this->encodeResult.count = 0; 
}

void rfir::module::ttl::Encoder::uninitEncodeResult() {
  delete this->encodeResult.result;
  this->encodeResult.result = 0;
  this->encodeResult.count = 0;
}

void rfir::module::ttl::Encoder::clearEncodeResult() {
   this->encodeResult.count = 0;
}

int  rfir::module::ttl::Encoder::getEncodeResultCount() {
  return this->encodeResult.count;
}

rfir::module::ttl::Encoder::EncodeResult* rfir::module::ttl::Encoder::getEncodeResult() {
  return &this->encodeResult;
}

int  rfir::module::ttl::Encoder::encodeData(Params p, uint64_t data, int nbits, uint16_t* result) {
  int offset = 0;

  if (p.MSBfirst) {
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1) {
        if (data & mask) {  // Send a 1
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                  result[offset++] = p.onespace;
                } else { //Event Mark
                  result[offset++] = p.onemark;
                }
            } else {
                result[offset++] = p.onemark;
                result[offset++] = p.onespace;
            }
        } else {  // Send a 0
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                  result[offset++] = p.zerospace;
                } else { //Event Mark
                  result[offset++] = p.zeromark;
                }              
            } else {
                result[offset++] = p.zeromark;
                result[offset++] = p.zerospace;
            }
        } 
    }
  } else {
    for (size_t i = 0; i < nbits; i++) {    
        if ((data >> i) & 1) {  // Send a 1
            if (p.step == 1){
                if (offset & 2) { //Odd Space
                  result[offset++] = p.onespace;
                } else { //Event Mark
                  result[offset++] = p.onemark;
                }
            } else {
                result[offset++] = p.onemark;
                result[offset++] = p.onespace;
            }
        } else {  // Send a 0
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                  result[offset++] = p.zerospace;
                } else { //Event Mark
                  result[offset++] = p.zeromark;
                }             
            } else {
                result[offset++] = p.zeromark;
                result[offset++] = p.zerospace;
            }
        } 
    }
  }

  return offset;
  
}

int  rfir::module::ttl::Encoder::encode(Params p, uint8_t* data, int nbits, uint16_t* result) {
  int offset = 0;

  if (p.headermark) result[offset++] = p.headermark;
  if (p.headerspace) result[offset++] = p.headerspace;
  for (size_t i = 0; i < nbits / 8; i++)
  {
    offset += encodeData(p, data[i], 8, result + offset);
  }
  if (nbits % 8) {
    offset += encodeData(p, data[nbits / 8], nbits % 8, result + offset);
  }
  
  if (p.footermark) result[offset++] = p.footermark;
  if (p.footerspace) result[offset++] = p.footerspace;  

  return offset;

}

int rfir::module::ttl::Encoder::encode(neb::CJsonObject* jblocks,  uint16_t* result) {
  int offset = 0;
  if (jblocks->IsArray()) {
      int count = jblocks->GetArraySize();   
      for (size_t i = 0; i < count; i++)
      {
          neb::CJsonObject bl;// = jblocks[i];
          neb::CJsonObject jp;
          jblocks->Get(i, bl);
          std::string data;
          if (bl.Get("data", data)) {       
            Params p;            
            if (i < this->getEncodeParamsCount())
              p = this->getEncodeParams()->params[i];
            
            if (bl.Get("params", jp)) {                    
              parseParams(&jp, &p);
            }

            uint8_t* dataBits = new uint8_t[256];

            int len = parseData(String(data.c_str()), dataBits, p.MSBfirst);
            offset += encode(p, dataBits, len, result + offset);

            delete dataBits;

          }
      }
  }
  return offset;
}

int rfir::module::ttl::Encoder::encode(neb::CJsonObject* blocks) {
  initEncodeResult();
  this->getEncodeResult()->count = encode(blocks, this->getEncodeResult()->result);
  if (this->onEncoded)
    this->onEncoded(this, this->getEncodeResult(), this->name);
  return this->getEncodeResultCount();
}

String  rfir::module::ttl::Encoder::toString() {
    String result;
 
    return result;

}

void  rfir::module::ttl::Encoder::setEncodeParams(EncodeParams* params, String name) {
    delete this->encodeParams.params;
    this->encodeParams.params = 0;
    this->encodeParams.count = 0;
    
    this->name = name;
    if (params) {
      this->encodeParams.params = new Params[params->count];
      this->encodeParams.count = params->count;
      for (size_t i = 0; i < params->count; i++)
      {
        this->encodeParams.params[i] = params->params[i];
      }      
    }
}

rfir::module::ttl::Encoder::EncodeParams* rfir::module::ttl::Encoder::getEncodeParams() {
  return &(this->encodeParams);
}

int rfir::module::ttl::Encoder::getEncodeParamsCount() {
  return this->encodeParams.count;
}

void rfir::module::ttl::Encoder::setJEncode(neb::CJsonObject* jencode) {
  rfir::module::ttl::Encoder::EncodeParams encode;
  rfir::module::ttl::Config::initDeviceEncode(jencode, &encode);  
  this->setEncodeParams(&encode);
  rfir::module::ttl::Config::uninitDeviceEncode(&encode);  

  this->jEncode = *jencode;
}

neb::CJsonObject* rfir::module::ttl::Encoder::getJEncode(neb::CJsonObject* jencode) {
  return &this->jEncode;
}

bool rfir::module::ttl::Encoder::parseParams(neb::CJsonObject* jp, rfir::module::ttl::Encoder::Params* p) {
    if (jp && p) {
        int       nbits = 0;
        int       headermark = 0;
        int       headerspace = 0;
        int       onemark = 0;
        int       onespace = 0;
        int       zeromark = 0;
        int       zerospace = 0;
        int       footermark = 0;
        int       footerspace = 0;
        int       step = 2;
        int       MSBfirst = 1;

        if (jp->Get("nbits", nbits))              p->nbits = nbits;
        if (jp->Get("headermark", headermark))    p->headermark = headermark;
        if (jp->Get("headerspace", headerspace))  p->headerspace = headerspace;
        if (jp->Get("onemark", onemark))          p->onemark = onemark;
        if (jp->Get("onespace", onespace))        p->onespace = onespace;
        if (jp->Get("zeromark", zeromark))        p->zeromark = zeromark;    
        if (jp->Get("zerospace", zerospace))      p->zerospace = zerospace;   
        if (jp->Get("footermark", footermark))    p->footermark = footermark;
        if (jp->Get("footerspace", footerspace))  p->footerspace = footerspace;
 
        if (jp->Get("MSBfirst", MSBfirst))        p->MSBfirst = MSBfirst;    
        if (jp->Get("step", step))                p->step = step;   
        return true;
    }

    return false;
}

int  rfir::module::ttl::Encoder::parseData(const char* data, int nbits, uint8_t* result, bool MSBFirst) {
    int offset = 0;
    for (size_t i = 0; i < nbits / 8; i++)
    {
      uint64_t byte = 0;
      offset += parseData(data + offset, 8, byte, MSBFirst);
      result[i] = (uint8_t)byte; 
    }

    if (nbits % 8) {
        uint64_t byte = 0;
        offset += parseData(data + offset, nbits % 8, byte, MSBFirst);
        result[offset / 8] = (uint8_t)byte; 
    }

    return offset;
}

int  rfir::module::ttl::Encoder::parseData(const char* data, int nbits, uint64_t& result, bool MSBFirst) {
  for (size_t i = 0; i < nbits; i++)
  {
    data[i] == '0' ? result <<= 1 : result = (result << 1) | 1;    
  }
  //if (!MSBFirst) result = rfir::util::Util::reverseBits(result, nbits);
  
  return  nbits;  

}

int  rfir::module::ttl::Encoder::parseData(String data, uint8_t* result, bool MSBFirst) {
  std::string str(data.c_str());
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
        String p = "0x" + data[i];
        int len = 4;        
        if (i + 1 < count) {
           p = p +  data[i + 1];
           len = 8;
        }

        char* str;   
        uint8_t byte = (uint8_t)strtol(p.c_str(), &str, 16);  
        //if (!MSBFirst) byte = rfir::util::Util::reverseBits(byte, len);
        result[(i - 2) / 2] = byte;
        nbits += len;
      }
    } else {
      nbits = parseData(data.c_str(), count, result, MSBFirst);
    }
  }

  return nbits;
}

uint16_t*  rfir::module::ttl::Encoder::parseRaw(const char* data, int size, int& count) {
  int cnt = 0;
  count = 0;
  for (size_t i = 0; i < size; i++)
  {
    char c = data[i];
    if (c != ',' ) cnt++;
  }

  uint16_t* result = new uint16_t[cnt];

  std::string str;
  for (size_t i = 0; i < size; i++)
  {    
    char c = data[i];
    if (c != ',' ) 
      str += c;
    else  {      
      result[count++] = atoi(str.c_str());
      str = "";
    }
  }

  return result;
}

std::string rfir::module::ttl::Encoder::packEncodedCmd(Encoder* encoder, EncodeResult* data, String name) {
    neb::CJsonObject jp, hd, pld, encode, blocks, raw;
    hd.Add("cmd", 10);
    hd.Add("stp", 1);
    
    for (size_t i = 0; i < encoder->getEncodeParamsCount(); i++)
    {
      neb::CJsonObject block;
      neb::CJsonObject params;
      auto ps = encoder->getEncodeParams();
      params.Parse(ps->params[i].toString());
      block.Add("params", params);
      blocks.Add(block);
    }
    encode.Add("blocks", blocks);
    encode.Add("raw", data->toString());
    pld.Add("encode", encode);

    
    jp.Add("hd", hd);
    jp.Add("pld", pld);
    return jp.ToString();   

}


std::string  rfir::module::ttl::Encoder::EncodeResult::toString() {
  String str;
  for (size_t i = 0; i < count; i++)
  {
    char c[10];
    uint16_t b = this->result[i];
    itoa(b, c, 10);
    str = str + String(c) + ",";    
  }
  
  return std::string(str.c_str());
}

std::string  rfir::module::ttl::Encoder::Params::toString() {
  neb::CJsonObject json;
  
  json.Add("nbits", nbits);
  json.Add("headermark", headermark);
  json.Add("headerspace", headerspace);
  json.Add("onemark", onemark);
  json.Add("onespace", onespace);
  json.Add("zeromark", zeromark);
  json.Add("zerospace", zerospace);
  json.Add("footermark", footermark);
  json.Add("footerspace", footerspace);
  json.Add("MSBfirst", MSBfirst);  
  json.Add("step", step); 
  return json.ToString();

}