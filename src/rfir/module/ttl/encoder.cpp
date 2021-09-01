#include "rfir.h"
#include <sys/time.h>
#include "rfir/util/util.h"

rfir::module::ttl::Encoder::Encoder(RFIR* rfir) {
  this->rfir = rfir;
}

rfir::module::ttl::Encoder::~Encoder() {  
  this->encodeParams.free();
  this->encodeResult.free();
  this->getEncodeParams()->free();
  uninitEncodeResult();
}


void rfir::module::ttl::Encoder::initEncodeResult() {
  uninitEncodeResult();
  int bufSize = this->rfir->sniffer->getSniffParams()->params.bufSize;
  this->encodeResult.result = new uint16_t[bufSize];
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

  if(!p.footermark && !p.footerspace && p.lastspace)
    result[offset-1] = p.lastspace;

  return offset;

}

int rfir::module::ttl::Encoder::encode(neb::CJsonObject* jblocks,  uint16_t* result) {
  int offset = 0;
  if (jblocks->IsArray()) {
      int count = jblocks->GetArraySize();   
      for (size_t i = 0; i < count; i++)
      {
          neb::CJsonObject bl;
          neb::CJsonObject jp;
          jblocks->Get(i, bl);
          std::string data;
          if (bl.Get("data", data)) {       
            Params p;            
            if (i < this->getEncodeParamsCount())
              p = this->getEncodeParams()->params[i];
            
            if (bl.Get("params", jp)) {      
              p.parseFromJson(&jp);
            }

            int bufSize = this->rfir->sniffer->getSniffParams()->params.bufSize;

            int size = bufSize / 8 + 1;

            uint8_t* dataBits = new uint8_t[size];

            int len = parseData(String(data.c_str()), dataBits, p.MSBfirst);
            offset += encode(p, dataBits, len, result + offset);

            delete dataBits;

          }
      }
  }
  return offset;
}

int rfir::module::ttl::Encoder::encode(neb::CJsonObject* jblocks) {
  rfir->encoder->getEncodeParams()->parseFromJson(jblocks);
  initEncodeResult();
  this->getEncodeResult()->count = encode(jblocks, this->getEncodeResult()->result);
  if (this->onEncoded)
    this->onEncoded(this, this->getEncodeResult());
  return this->getEncodeResultCount();
}

int rfir::module::ttl::Encoder::encode(rfir::module::ttl::Decoder::DecodeResults* data) {
  if (data->count != this->getEncodeParams()->count)
    return 0;

  initEncodeResult();
  this->getEncodeResult()->count = encode(data, this->getEncodeResult()->result);
  if (this->onEncoded)
    this->onEncoded(this, this->getEncodeResult());
  return this->getEncodeResultCount();
}

int rfir::module::ttl::Encoder::encode(rfir::module::ttl::Decoder::DecodeResults* results, uint16_t* result) {
  int offset = 0;
  int bufSize = this->rfir->sniffer->getSniffParams()->params.bufSize;
  int size = bufSize / 8 + 1;
  uint8_t* dataBits = new uint8_t[size];

  for (size_t i = 0; i < results->count; i++)
  {
    auto p = this->getEncodeParams()->params[i];
    std::string data = results->result[i].toBitString();
    int len = parseData(String(data.c_str()), dataBits, p.MSBfirst);
    offset += encode(p, dataBits, len, result + offset);
  }

  delete dataBits;

  return offset;

}

std::string  rfir::module::ttl::Encoder::toString() {
  return this->getEncodeResult()->toString();
}


rfir::module::ttl::Encoder::EncodeParams* rfir::module::ttl::Encoder::getEncodeParams() {
  return &(this->encodeParams);
}

int rfir::module::ttl::Encoder::getEncodeParamsCount() {
  return this->encodeParams.count;
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
        char* str;
        String p = data.substring(i, i + 2);
        int len = 4 * p.length();   
       
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

std::string rfir::module::ttl::Encoder::packEncodedCmd(Encoder* encoder, EncodeResult* data) {
    neb::CJsonObject jp, hd, pld, encode, blocks, raw;
    hd.Add("did", rfir::util::Util::GetChipId());
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
    pld.Add("device", encoder->name);
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

void rfir::module::ttl::Encoder::EncodeResult::free() {
  delete this->result;
  this->result = 0;
  this->count = 0;
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
  json.Add("lastspace", lastspace);
  return json.ToString();

}

bool rfir::module::ttl::Encoder::Params::parseFromJson(neb::CJsonObject* jp) {
    if (jp) {
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
        int       lastspace = 0;
        int       MSBfirst = 1;

        if (jp->Get("nbits", nbits))              this->nbits = nbits;
        if (jp->Get("headermark", headermark))    this->headermark = headermark;
        if (jp->Get("headerspace", headerspace))  this->headerspace = headerspace;
        if (jp->Get("onemark", onemark))          this->onemark = onemark;
        if (jp->Get("onespace", onespace))        this->onespace = onespace;
        if (jp->Get("zeromark", zeromark))        this->zeromark = zeromark;    
        if (jp->Get("zerospace", zerospace))      this->zerospace = zerospace;   
        if (jp->Get("footermark", footermark))    this->footermark = footermark;
        if (jp->Get("footerspace", footerspace))  this->footerspace = footerspace;
 
        if (jp->Get("MSBfirst", MSBfirst))        this->MSBfirst = MSBfirst;    
        if (jp->Get("step", step))                this->step = step;   
        if (jp->Get("lastspace", lastspace))      this->lastspace = lastspace;   
        return true;
    }

    return false;
}

bool rfir::module::ttl::Encoder::Params::clone(Decoder::Params* p) {
    if (p) {
        this->nbits = p->nbits;
        this->headermark = p->headermark;
        this->headerspace = p->headerspace;
        this->onemark = p->onemark;
        this->onespace = p->onespace;
        this->zeromark = p->zeromark;    
        this->zerospace = p->zerospace;   
        this->footermark = p->footermark;
        this->footerspace = p->footerspace;
 
        this->MSBfirst = p->MSBfirst;    
        this->step = p->step;   
        this->lastspace = p->lastspace;   
        return true;
    }
    return false;
}

bool rfir::module::ttl::Encoder::EncodeParams::parseFromJson(neb::CJsonObject* jencode) {
  neb::CJsonObject* jblocks = 0;
  neb::CJsonObject  blocks;
  int response = 0;
  if (jencode && jencode->Get("response", response)) {    
      this->response = response;     
  }

  if (jencode) {
    if (jencode->IsArray())
      jblocks = jencode;
    else {
      if (jencode->Get("blocks", blocks))
        jblocks = &blocks;
    }
  }

  if (jblocks && jblocks->IsArray()) {
    int count = jblocks->GetArraySize();  
    if (count > 0) {  
      if (this->count != count) {
        this->free();    
        this->params = new Params[count];
        this->count = count;
      }
      
      for (size_t i = 0; i < count; i++)
      {
        neb::CJsonObject bl;
        neb::CJsonObject jp;
        jblocks->Get(i, bl);
        if (bl.Get("params", jp))
          (this->params + i)->parseFromJson(&jp);
      }
    }
    return true;    
  }
  Serial.println("EncodeParams->parseFromJson: Failed");
  return false;
}

void rfir::module::ttl::Encoder::EncodeParams::free() {
  delete this->params;
  this->params = 0;
  this->count = 0;

}

bool rfir::module::ttl::Encoder::EncodeParams::clone(EncodeParams* p) {
  if (p) {
    free();
    this->params = new Params[p->count];
    this->count = p->count;
    this->response = p->response;
    for (size_t i = 0; i < p->count; i++)
    {
      this->params[i] = p->params[i];
    } 
    return 1;
  }

  return 0;

}

bool rfir::module::ttl::Encoder::EncodeParams::clone(Decoder::DecodeParams* p) {
  if (p) {
    free();
    this->params = new Params[p->count];
    this->count = p->count;
    for (size_t i = 0; i < p->count; i++)
    {
      (this->params + i)->clone(p->params + i);
    } 
    return 1;
  }

  return 0;

}

int rfir::module::ttl::Encoder::EncodeParams::create(int size) {
  free();
  this->params = new Params[size];
  this->count = size;
  return this->count;
}

