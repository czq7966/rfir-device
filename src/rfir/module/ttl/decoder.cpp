#include "rfir.h"
#include <sys/time.h>

rfir::module::ttl::Decoder::Decoder(RFIR* rfir) {
  this->rfir = rfir;
}

rfir::module::ttl::Decoder::~Decoder() {  
  this->decodeParams.free();
  this->decodeResults.free();
  this->getDecodeParams()->free();
  uninitDecodeResults();
}


void rfir::module::ttl::Decoder::initDecodeResults() {
  uninitDecodeResults();
  int count = this->getDecodeParamsCount();
  this->decodeResults.count = count;
  this->decodeResults.result = new DecodeResult[count];
  for (size_t i = 0; i < count; i++)
  {
    auto p = this->decodeParams.params + i;
    auto r = this->decodeResults.result + i;
    r->bits = 0;
    r->bytes = 0;
    r->nbits = 0;
    if (!p->use_bits) {
      int nbytes = p->nbits / 8;
      if (p->nbits % 8)
        nbytes++;
      r->bytes = new uint8_t[nbytes];
   }
  }  
}

void rfir::module::ttl::Decoder::uninitDecodeResults() {
  if (this->decodeResults.result) {
    int count = this->getDecodeResultsCount();
    for (size_t i = 0; i < count; i++)
    {
      auto r = this->decodeResults.result + i;
      delete r->bytes;
      r->bytes = 0;
      r->bits = 0;
      r->nbits = 0;
    }

    delete this->decodeResults.result;
    this->decodeResults.result = 0;
    this->decodeResults.count = 0;
  }
}

void rfir::module::ttl::Decoder::clearDecodeResults() {
  if (this->decodeResults.result) {
    int count = this->getDecodeResultsCount();
    for (size_t i = 0; i < count; i++)
    {
      auto r = this->decodeResults.result + i;
      r->bits = 0;
      r->nbits = 0;
    }
  }  
}

int  rfir::module::ttl::Decoder::getDecodeResultsCount() {
  return this->decodeResults.count;
}

rfir::module::ttl::Decoder::DecodeResults* rfir::module::ttl::Decoder::getDecodeResults() {
  return &this->decodeResults;
}

int  rfir::module::ttl::Decoder::_decode(uint16_t* data, int size) {
  initDecodeResults();
  //int offset = data[0] > 1 ? 0 : (data[0] == 1 ? 1 : 2);
  int offset = data[0] == 1 ? 1 : 0;
  size = size - offset;

  bool matched = 0;
  while (offset < size)
  {
    int matchCount = 0;
    for (size_t i = 0; i < getDecodeParamsCount(); i++)
    {
      Params* p = this->getDecodeParams()->params + i;
      DecodeResult* r = this->decodeResults.result + i;       
      p->step = p->step == 1 ? 1 : 2;

      int matched = this->matchGeneric(data + offset + matchCount, &(r->bits), r->bytes, size - offset - matchCount, *p);

      if (matched) {
        matchCount += matched;
        r->nbits = p->nbits;
      }
      else {        
        matchCount = 0;
        break;
      }
    }   

    if (matchCount) {
      offset += matchCount;
      matched = 1;
      if (onDecoded) 
          onDecoded(this, getDecodeResults());
      break;
    }
    else
      offset += 2;
  }

  if (matched)
    return offset;

  return 0;
}

int  rfir::module::ttl::Decoder::decode(uint16_t* data, int size, int maxTimes) {
    int offset = 0;
    int times = 0;
    while (offset < size) {
        int matched = this->_decode(data + offset, size - offset);
        if (!matched) 
            break;
        offset += matched;

        times++;
        if (times >= maxTimes)
          break;
    }

    return offset;
}

int  rfir::module::ttl::Decoder::decode() {
  if (this->rfir) {
    int count = rfir->sniffer->sniffDelta();
    if (count > 0) {
        auto data = (uint16_t*)(rfir->sniffer->getSniffedDelta());
        int offset = decode(data, count);
        rfir->sniffer->resetSniff();    
        return offset;
    }  
  }
  return 0;
}

String  rfir::module::ttl::Decoder::toString() {
    String result;
    for (size_t i = 0; i < getDecodeResultsCount(); i++)
    {
      Params* p = this->decodeParams.params + i;
      DecodeResult* r = this->decodeResults.result + i; 
      String bitsStr, bytesStr;
      if (r->nbits > 0) {
        if (p->use_bits) {
          uint64_t bits = r->bits;
          for (size_t j = 0; j < r->nbits; j++)
          {
              bitsStr = (bits & 1 ? "1": "0") + bitsStr;
              bits >>= 1;
          }
        } else {
          for (size_t j = 0; j < r->nbits / 8; j++)
          {
              uint8_t b = r->bytes[j];  
              String bitStr;
              for (size_t k = 0; k < 8; k++)
              {
                  bitStr = String(b & 1 ? 1: 0) + bitStr;
                  b >>= 1;    
              }   

              bitsStr = bitsStr + bitStr;
          }

          for (size_t j = 0; j < r->nbits / 8; j++)
          {
              char c[3];
              uint8_t b = r->bytes[j];  
              itoa(b, c, 16);

              bytesStr = bytesStr + ((strlen(c) == 1) ? ("0" + String(c)) :  String(c));
          }
        }
      }

      result = result + bitsStr + "," + bytesStr + ";";
    }     
    return result;

}

rfir::module::ttl::Decoder::DecodeParams* rfir::module::ttl::Decoder::getDecodeParams() {
  return &this->decodeParams;
}

int rfir::module::ttl::Decoder::getDecodeParamsCount() {
  return this->decodeParams.count;
}



uint32_t rfir::module::ttl::Decoder::ticksLow(const uint32_t usecs, const uint8_t tolerance, const uint16_t delta) {
    return ((uint32_t)std::max((int32_t)(usecs * (1.0 - (tolerance) / 100.0) - delta),0));
}

uint32_t rfir::module::ttl::Decoder::ticksHigh(const uint32_t usecs, const uint8_t tolerance,const uint16_t delta) {
  return ((uint32_t)(usecs * (1.0 + (tolerance) / 100.0)) + 1 + delta);  
}

bool rfir::module::ttl::Decoder::match(uint32_t measured, uint32_t desired, uint8_t tolerance, uint16_t delta) {
    return (measured >= ticksLow(desired, tolerance, delta) &&
        measured <= ticksHigh(desired, tolerance, delta));
}

bool rfir::module::ttl::Decoder::matchAtLeast(uint32_t measured, uint32_t desired, uint8_t tolerance, uint16_t delta) {
    if (measured == 0) return true;
    // return measured >= ticksLow(std::min(desired, 15 * 1000U), tolerance, delta);
    return measured >= ticksLow(desired, tolerance, delta);
}

bool rfir::module::ttl::Decoder::matchMark(uint32_t measured, uint32_t desired, uint8_t tolerance, int16_t excess) {
    return match(measured, desired + excess, tolerance);
}

bool rfir::module::ttl::Decoder::matchMarkRange(const uint32_t measured, const uint32_t desired, const uint16_t range, const int16_t excess) {
    return match(measured, desired + excess, 0, range);
}

bool rfir::module::ttl::Decoder::matchSpace(const uint32_t measured, const uint32_t desired, const uint8_t tolerance, const int16_t excess) {
    return match(measured, desired - excess, tolerance);
}

bool rfir::module::ttl::Decoder::matchSpaceRange(const uint32_t measured, const uint32_t desired, const uint16_t range, const int16_t excess) {
    return match(measured, desired - excess, 0, range);
}

uint16_t rfir::module::ttl::Decoder::compare(const uint16_t oldval, const uint16_t newval) {
    if (newval < oldval * 0.8)
        return 0;
    else if (oldval < newval * 0.8)
        return 2;
    else
        return 1;
}

uint64_t rfir::module::ttl::Decoder::reverseBits(uint64_t input, uint16_t nbits) {
 return rfir::util::Util::reverseBits(input, nbits);
}

uint16_t rfir::module::ttl::Decoder::_matchGeneric(volatile uint16_t *data_ptr,
                              uint64_t *result_bits_ptr,
                              uint8_t *result_bytes_ptr,
                              const bool use_bits,
                              const uint16_t remaining,
                              const uint16_t nbits,
                              const uint16_t hdrmark,
                              const uint32_t hdrspace,
                              const uint16_t onemark,
                              const uint32_t onespace,
                              const uint16_t zeromark,
                              const uint32_t zerospace,
                              const uint16_t footermark,
                              const uint32_t footerspace,
                              const uint8_t  tolerance,
                              const int16_t  excess,
                              const bool     atleast,                              
                              const bool     MSBfirst,
                              const uint8_t  step,
                              const uint16_t lastspace) {


  if (!use_bits && nbits % 8 != 0)  return 0;
  // Calculate if we expect a trailing space in the data section.
  const bool kexpectspace = lastspace ? 0 : (footermark || (onespace != zerospace));
  // Calculate how much remaining buffer is required.
  uint16_t min_remaining = nbits * step - (kexpectspace ? 0 : 1);

  if (hdrmark) min_remaining++;
  if (hdrspace) min_remaining++;
  if (footermark) min_remaining++;
  // Don't need to extend for footerspace because it could be the end of message

  // Check if there is enough capture buffer to possibly have the message.
  if (remaining < min_remaining) return 0;  // Nope, so abort.
  uint16_t offset = 0;

  // Header
  if (hdrmark && !matchMark(*(data_ptr + offset++), hdrmark, tolerance, excess))
    return 0;
  if (hdrspace && !matchSpace(*(data_ptr + offset++), hdrspace, tolerance,
                              excess))
    return 0;

  // Data
  if (use_bits) {  // Bits.
    match_result_t result = matchData(data_ptr + offset, nbits,
                                              onemark, onespace,
                                              zeromark, zerospace, tolerance,
                                              excess, MSBfirst, kexpectspace, step);
    if (!result.success) return 0;
    *result_bits_ptr = result.data;
    offset += result.used;
  } else {  // bytes
    uint16_t data_used = matchBytes(data_ptr + offset, result_bytes_ptr,
                                            remaining - offset, nbits / 8,
                                            onemark, onespace,
                                            zeromark, zerospace, tolerance,
                                            excess, MSBfirst, kexpectspace, step);
    if (!data_used) return 0;
    offset += data_used;
  }
  // Footer
  if (footermark && !matchMark(*(data_ptr + offset++), footermark, tolerance,
                               excess))
    return 0;

  // If we have something still to match & haven't reached the end of the buffer
  if (footerspace && offset < remaining) {
      if (atleast) {
        if (!matchAtLeast(*(data_ptr + offset), footerspace, tolerance, excess))
          return 0;
      } else {
        if (!matchSpace(*(data_ptr + offset), footerspace, tolerance, excess))
          return 0;
      }
      offset++;
  }
  return offset;

}


rfir::module::ttl::Decoder::match_result_t rfir::module::ttl::Decoder::matchData(volatile uint16_t *data_ptr, const uint16_t nbits,
                           const uint16_t onemark, const uint32_t onespace,
                           const uint16_t zeromark, const uint32_t zerospace,
                           const uint8_t tolerance,
                           const int16_t excess,
                           const bool MSBfirst,
                           const bool expectlastspace,
                           const uint8_t step) {
  if (step == 1) {
    return matchDataStep1(data_ptr, nbits, onemark, onespace, zeromark, zerospace, tolerance, excess, MSBfirst, expectlastspace, step);
  }

  match_result_t result;
  result.success = false;  // Fail by default.
  result.data = 0;
  if (expectlastspace) {  // We are expecting data with a final space.
    for (result.used = 0; result.used < nbits * step;
         result.used += step, data_ptr += step) {
      // Is the bit a '1'?
      if (matchMark(*data_ptr, onemark, tolerance, excess) &&
          matchSpace(*(data_ptr + 1), onespace, tolerance, excess)) {
        result.data = (result.data << 1) | 1;
      } else if (matchMark(*data_ptr, zeromark, tolerance, excess) &&
                 matchSpace(*(data_ptr + 1), zerospace, tolerance, excess)) {
        result.data <<= 1;  // The bit is a '0'.
      } else {
        if (!MSBfirst) result.data = reverseBits(result.data, result.used / step);
        return result;  // It's neither, so fail.
      }
    }
    result.success = true;
  } else {  //  data without a final space.
    // Match all but the last bit, as it may not match easily.
    result = matchData(data_ptr, nbits ? nbits - 1 : 0, onemark, onespace,
                       zeromark, zerospace, tolerance, excess, true, true, step);
    if (result.success) {
      // Is the bit a '1'?
      if (matchMark(*(data_ptr + result.used), onemark, tolerance, excess))
        result.data = (result.data << 1) | 1;
      else if (matchMark(*(data_ptr + result.used), zeromark, tolerance,
               excess))
        result.data <<= 1;  // The bit is a '0'.
      else
        result.success = false;
      if (result.success) result.used += step;
    }
  }
  if (!MSBfirst) result.data = reverseBits(result.data, nbits);
  return result;
}

rfir::module::ttl::Decoder::match_result_t rfir::module::ttl::Decoder::matchDataStep1(volatile uint16_t *data_ptr, const uint16_t nbits,
                           const uint16_t onemark, const uint32_t onespace,
                           const uint16_t zeromark, const uint32_t zerospace,
                           const uint8_t tolerance,
                           const int16_t excess,
                           const bool MSBfirst,
                           const bool expectlastspace,
                           const uint8_t step) {

  match_result_t result;
  result.success = false;  // Fail by default.
  result.data = 0;
  if (expectlastspace) {  // We are expecting data with a final space.
    for (result.used = 0; result.used < nbits * step;
         result.used += step, data_ptr += step) {
      if (result.used % 2) { //Odd Space
        if (matchSpace(*data_ptr, onespace, tolerance, excess))
          result.data = (result.data << 1) | 1; // '1'
        else if (matchSpace(*data_ptr, zerospace, tolerance, excess))
          result.data <<= 1;  // '0'
        else {
          if (!MSBfirst) result.data = reverseBits(result.data, result.used / step);
          return result;  // It's neither, so fail.
        }
      } else { //Event Mark
        if (matchMark(*data_ptr, onemark, tolerance, excess))
          result.data = (result.data << 1) | 1; // '1'
        else if (matchMark(*data_ptr, zeromark, tolerance, excess))
          result.data <<= 1;  // '0'
        else {
          if (!MSBfirst) result.data = reverseBits(result.data, result.used / step);
          return result;  // It's neither, so fail.
        }
      }
    }
    result.success = true;
  } else {  //  data without a final space.
    // Match all but the last bit, as it may not match easily.
    result = matchData(data_ptr, nbits ? nbits - 1 : 0, onemark, onespace,
                       zeromark, zerospace, tolerance, excess, true, true, step);
    if (result.success) {
      // Is the bit a '1'?
      if (matchMark(*(data_ptr + result.used), onemark, tolerance, excess))
        result.data = (result.data << 1) | 1;
      else if (matchMark(*(data_ptr + result.used), zeromark, tolerance,
               excess))
        result.data <<= 1;  // The bit is a '0'.
      else
        result.success = false;
      if (result.success) result.used++;
    }
  }
  if (!MSBfirst) result.data = reverseBits(result.data, nbits);
  return result;
}



uint16_t rfir::module::ttl::Decoder::matchBytes(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                                    const uint16_t remaining, const uint16_t nbytes,
                                    const uint16_t onemark, const uint32_t onespace,
                                    const uint16_t zeromark, const uint32_t zerospace,
                                    const uint8_t tolerance,
                                    const int16_t excess,
                                    const bool MSBfirst,                                    
                                    const bool expectlastspace,
                                    const uint8_t step) {
  // Check if there is enough capture buffer to possibly have the desired bytes.
  if (remaining  < (nbytes * 8 * step) )
    return 0;  // Nope, so abort.
  uint16_t offset = 0;
  for (uint16_t byte_pos = 0; byte_pos < nbytes; byte_pos++) {
    bool lastspace = (byte_pos + 1 == nbytes) ? expectlastspace : true;
    match_result_t result = matchData(data_ptr + offset, 8, onemark, onespace,
                                      zeromark, zerospace, tolerance, excess,
                                      MSBfirst, lastspace, step);
    if (result.success == false) return 0;  // Fail
    
    result_ptr[byte_pos] = (uint8_t)result.data;
    offset += result.used;
  }
  return offset;

}  


uint16_t rfir::module::ttl::Decoder::matchGeneric(volatile uint16_t *data_ptr,
                        uint64_t *result_ptr,
                        const uint16_t remaining, const uint16_t nbits,
                        const uint16_t hdrmark, const uint32_t hdrspace,
                        const uint16_t onemark, const uint32_t onespace,
                        const uint16_t zeromark, const uint32_t zerospace,
                        const uint16_t footermark, const uint32_t footerspace,
                        const bool atleast,
                        const uint8_t tolerance,
                        const int16_t excess,
                        const bool MSBfirst,
                        const uint8_t step,
                        const uint16_t exceptlastspace) {

  return _matchGeneric(data_ptr, result_ptr, NULL, true, remaining, nbits,
                       hdrmark, hdrspace, onemark, onespace,
                       zeromark, zerospace, footermark, footerspace,
                       tolerance, excess, atleast, MSBfirst, step, exceptlastspace);
}
uint16_t rfir::module::ttl::Decoder::matchGeneric(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                        const uint16_t remaining, const uint16_t nbits,
                        const uint16_t headermark, const uint32_t headerspace,
                        const uint16_t onemark, const uint32_t onespace,
                        const uint16_t zeromark, const uint32_t zerospace,
                        const uint16_t footermark,
                        const uint32_t footerspace,
                        const bool atleast,
                        const uint8_t tolerance,
                        const int16_t excess,
                        const bool MSBfirst,
                        const uint8_t step,
                        const uint16_t exceptlastspace) {
  return _matchGeneric(data_ptr, NULL, result_ptr, false, remaining, nbits,
                       headermark, headerspace, onemark, onespace,
                       zeromark, zerospace, footermark, footerspace, 
                       tolerance, excess, atleast, MSBfirst, step, exceptlastspace);
}

uint16_t rfir::module::ttl::Decoder::matchGeneric(volatile uint16_t *data_ptr, uint64_t *result_bits_ptr, uint8_t *result_ptr,
                                        const uint16_t remaining, const Params params) {

  return _matchGeneric(data_ptr, result_bits_ptr, result_ptr, params.use_bits, remaining,  params.nbits,
                       params.headermark, params.headerspace, params.onemark, params.onespace,
                       params.zeromark, params.zerospace, params.footermark, params.footerspace, 
                       params.tolerance, params.excess, params.atleast, params.MSBfirst, params.step, params.lastspace);
}



std::string rfir::module::ttl::Decoder::packDecodedCmd(Decoder* decoder, DecodeResults* data) {
    neb::CJsonObject jp, hd, pld, decode, blocks;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", 10);
    hd.Add("stp", 1);
    
    for (size_t i = 0; i < data->count; i++)
    {
      neb::CJsonObject block;
      neb::CJsonObject params;
      auto ps = decoder->getDecodeParams();
      params.Parse(ps->params[i].toString());
      block.Add("params", params);
      block.Add("data", data->result[i].toBitString() + "," + data->result[i].toHexString());
      blocks.Add(block);
    }
    decode.Add("blocks", blocks);
    pld.Add("device", decoder->name);
    pld.Add("decode", decode);

    
    jp.Add("hd", hd);
    jp.Add("pld", pld);
    return jp.ToString();   
}


std::string  rfir::module::ttl::Decoder::DecodeResult::toBitString() {
    String result;
    if (this->nbits > 0) {
      if (!this->bytes) {
        uint64_t bits = this->bits;
        for (size_t j = 0; j < this->nbits; j++)
        {
            result = (bits & 1 ? "1": "0") + result;
            bits >>= 1;
        }
      } else {
        for (size_t j = 0; j < this->nbits / 8; j++)
        {
            uint8_t b = this->bytes[j];  
            String bitStr;
            for (size_t k = 0; k < 8; k++)
            {
                bitStr = String(b & 1 ? 1: 0) + bitStr;
                b >>= 1;    
            }   

            if (result.length() > 0)
               result = result + (" " + bitStr);
            else result = result + bitStr; 
        }
      }
    }

    return std::string(result.c_str());
}

std::string  rfir::module::ttl::Decoder::DecodeResult::toHexString() {
    String result;
    if (this->nbits > 0) {
      if (this->bytes) {
        for (size_t j = 0; j < this->nbits / 8; j++)
        {
            char c[3];
            uint8_t b = this->bytes[j];  
            itoa(b, c, 16);
            String hex = (strlen(c) == 1) ? ("0" + String(c)) :  String(c);

            if (result.length() > 0)
              result = result + (" " + hex);
            else
              result = result + hex; 
        }
      }
    }
    return std::string(result.c_str());
}

void rfir::module::ttl::Decoder::DecodeResult::free() {
  delete this->bytes;
  this->bits = 0;
  this->bytes = 0;
  this->nbits = 0;
}

void rfir::module::ttl::Decoder::DecodeResults::free() {
  for (size_t i = 0; i < count; i++)
  {
    (this->result + i)->free();
  }
  
  delete this->result;
  this->result = 0;
  this->count = 0;
}

std::string  rfir::module::ttl::Decoder::Params::toString() {
  neb::CJsonObject json;
  json.Add("use_bits", use_bits);
  json.Add("nbits", nbits);
  json.Add("headermark", headermark);
  json.Add("headerspace", headerspace);
  json.Add("onemark", onemark);
  json.Add("onespace", onespace);
  json.Add("zeromark", zeromark);
  json.Add("zerospace", zerospace);
  json.Add("footermark", footermark);
  json.Add("footerspace", footerspace);
  json.Add("tolerance", tolerance);
  json.Add("excess", excess);
  json.Add("atleast", atleast);  
  json.Add("MSBfirst", MSBfirst);  
  json.Add("step", step);
  json.Add("lastspace", lastspace);  
  return json.ToString();

}

bool rfir::module::ttl::Decoder::Params::parseFromJson(neb::CJsonObject* jp) {
    if (jp) {
        bool      use_bits = true;
        int       nbits = 32;
        int       headermark = 9000;
        int       headerspace = 4500;
        int       onemark = 600;
        int       onespace = 1600;
        int       zeromark = 600;
        int       zerospace = 550;
        int       footermark = 600;
        int       footerspace = 18000;
        int       tolerance = 30;
        int       excess = 0;
        int       atleast = true;                              
        int       MSBfirst = true;
        int       step = 2;
        int       lastspace = 0;

        if (jp->Get("use_bits", use_bits))        this->use_bits = use_bits;
        if (jp->Get("nbits", nbits))              this->nbits = nbits;

        if (jp->Get("headermark", headermark))    this->headermark = headermark;
        if (jp->Get("headerspace", headerspace))  this->headerspace = headerspace;
        if (jp->Get("onemark", onemark))          this->onemark = onemark;
        if (jp->Get("onespace", onespace))        this->onespace = onespace;
        if (jp->Get("zeromark", zeromark))        this->zeromark = zeromark;    
        if (jp->Get("zerospace", zerospace))      this->zerospace = zerospace;   
        if (jp->Get("footermark", footermark))    this->footermark = footermark;
        if (jp->Get("footerspace", footerspace))  this->footerspace = footerspace;

        if (jp->Get("tolerance", tolerance))      this->tolerance = tolerance;
        if (jp->Get("excess", excess))            this->excess = excess;
        if (jp->Get("atleast", atleast))          this->atleast = atleast;    
        if (jp->Get("MSBfirst", MSBfirst))        this->MSBfirst = MSBfirst;    
        if (jp->Get("step", step))                this->step = step;    
        if (jp->Get("lastspace", lastspace))      this->lastspace = lastspace;    
        
        this->use_bits = this->nbits % 8;

        return true;
    }

    return false;  
}


bool rfir::module::ttl::Decoder::DecodeParams::parseFromJson(neb::CJsonObject* jdecode) {
  neb::CJsonObject* jblocks = 0;
  neb::CJsonObject  blocks;
  int response = 0;
  if (jdecode && jdecode->Get("response", response)) {    
      this->response = response;     
  }

  if (jdecode) {
    if (jdecode->IsArray())
      jblocks = jdecode;
    else {
      if (jdecode->Get("blocks", blocks))
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
      return true;    
    } 
  }

  Serial.println("DecodeParams->parseFromJson: Failed"); 
  return false;
}

void rfir::module::ttl::Decoder::DecodeParams::free() {
  delete this->params;
  this->params = 0;
  this->count = 0;
}

bool rfir::module::ttl::Decoder::DecodeParams::clone(DecodeParams* p) {
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

int rfir::module::ttl::Decoder::DecodeParams::create(int size) {
  free();
  this->params = new Params[size];
  this->count = size;
}

std::string rfir::module::ttl::Decoder::DecodeParams::toString() {
  std::string str;
  for (size_t i = 0; i < count; i++)
  {
    if (str.length() > 0)
      str = str + "," + (this->params + i)->toString();
    else
      str = (this->params + i)->toString();
  }
  return str;
  
}