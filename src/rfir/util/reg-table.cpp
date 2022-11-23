#include "reg-table.h"
#include "debuger.h"

bool rfir::util::RegTable::encode(char* buf, int& size, std::list<int>& ids){
    char* sbuf = buf;
    uint8_t step = 2;
    for (auto it = ids.begin(); it != ids.end(); it++)
    {
        int key = *it;
        int value;       

        if (this->tables.get(key, value)) {            
            memcpy(buf, &key, step);
            buf = buf + step;
            if (key >= this->strMinNum && key <= this->strMaxNum ||
                key >= this->bytesMinNum && key <= this->bytesMaxNum
            ) {
                if (value >= 0) {   //Buffer内存
                    int len = 0;
                    if (key >= this->strMinNum && key <= this->strMaxNum)
                        len = strlen((const char*) value);
                    else this->sizes.get(key, len);

                    memcpy(buf, &len, step);
                    buf = buf + step;
                    if (len > 0) {
                        memcpy(buf, (void*)value, len);
                        buf = buf + len ;                
                    } 
                } else {    //vector<uint8_t> 对象
                    auto data = this->decodeVectorAddress(value);
                    int len = data->size();
                    memcpy(buf, &len, step);
                    buf = buf + step;
                    if (len > 0) {
                        for (size_t i = 0; i < len; i++)
                        {
                            buf[i] = (char)((*data)[i]);
                        }
                        buf = buf + len ; 
                    } 
                }
            } else {
                memcpy(buf, &value, step);
                buf = buf + step;
            }
        }        
    }
    
    size = buf - sbuf;
    return true;
}

bool rfir::util::RegTable::encode(char* buf, int& size){
    std::list<int> ids;
    this->tables.getKeys(ids);
    return this->encode(buf, size, ids);
};

bool rfir::util::RegTable::decode(const char* buf, int size, std::list<int>& ids){
    this->clear();
    const char* sbuf = buf;
    uint8_t step = 2;
    while(buf - sbuf < size) {
        uint16_t key;
        memcpy(&key, buf, step);
        buf = buf + step;
        if (key >= this->strMinNum && key <= this->strMaxNum ||
            key >= this->bytesMinNum && key <= this->bytesMaxNum
        ) {
            uint16_t len;            
            memcpy(&len, buf, step);
            buf = buf + step;            
            int value = len > 0 ? (int)buf : 0;
            this->tables.add(key, value);
            this->sizes.add(key, len);
            ids.push_back(key);
            buf = buf + len;
        } else {
            uint16_t value;
            memcpy(&value, buf, step);
            buf = buf + step;
            this->tables.add(key, value);
            ids.push_back(key);
        }        
    }
    return true;
}

bool rfir::util::RegTable::decode(const char* buf, int size){
    std::list<int> ids;
    return this->decode(buf, size, ids);
};    

bool rfir::util::RegTable::merge(RegTable* regtable, std::list<int>& ids){
    auto map = regtable->tables.getMap();
    for (auto  it = map->begin(); it != map->end(); it++)
    {
        int key = it->first;                    
        if (key >= this->strMinNum && key <= this->strMaxNum ||
            key >= this->bytesMinNum && key <= this->bytesMaxNum        
        ) {
            int value = 0;
            int len = 0;
            if (this->tables.get(key, value) && regtable->sizes.get(key, len) && value) {                
                auto buf = (const char*)it->second;
                if (value > 0) {    //Buffer
                    if (buf) memcpy((void*)value, buf , len);
                    if (key >= this->strMinNum && key <= this->strMaxNum )
                        ((char*)value)[len] = '\0';
                } else {    //Vector<uint8_t>                    
                    auto data = this->decodeVectorAddress(value);
                    data->clear();
                    if (buf) data->insert(data->end(), buf, buf + len);
                }
            }
            this->sizes.add(key, len);
            ids.push_back(key);    
        } else {
            this->tables.add(key, it->second);
            ids.push_back(key);
        } 
    }
    
    return true;
};


bool rfir::util::RegTable::load(const char* buf, int size, std::list<int>& ids){
    RegTable regtable;
    regtable.decode(buf, size);
    return this->merge(&regtable, ids);
}


bool rfir::util::RegTable::load(const char* buf, int size, rfir::util::MapFactory<int, int>& keys){
    std::list<int> ids;
    auto result = this->load(buf, size, ids);
    
    for (auto it = ids.begin(); it != ids.end(); it++)
    {
        keys.add(*it, 0);
    }
    
    return result;
};

void rfir::util::RegTable::clear(){
    this->tables.clear();
    this->sizes.clear();
};

void rfir::util::RegTable::dump(){
    std::list<int> ids;    
    this->tables.getKeys(ids);
    auto map = this->tables.getMap();
    GDebuger.println("rfir::util::RegTable::dump start:");
    for (auto it = map->begin(); it != map->end(); it++)
    {
        auto key = it->first;
        auto value = it->second;
        GDebuger.print(key);
        GDebuger.print(":");
        if (key >= this->strMinNum && key <= this->strMaxNum) {
            GDebuger.println((char*)value);
        } else if (key >= this->bytesMinNum && key <= this->bytesMaxNum) {
            auto len = this->sizes.get(key);
            if (value > 0) {               
                GDebuger.print(F(" Buffer: "));
                GDebuger.print(value);
            } else if (value < 0) {
                auto vec = this->decodeVectorAddress(value);
                len = vec->size();
                GDebuger.print(F(" Vector: "));
                GDebuger.print(value);
            } else {
               GDebuger.print(F(" Null: "));
               GDebuger.print(value);
            }

            GDebuger.print(F(", len: "));
            GDebuger.print(len);
            GDebuger.println(F(" "));            
        } else {
            GDebuger.print(value);
            GDebuger.println("");
        }        
    }
    GDebuger.println("rfir::util::RegTable::dump end:");
};


int rfir::util::RegTable::get(int key, RegTable* regtable){
    if (regtable && regtable->tables.exist(key))
        return regtable->tables.get(key);
    else   
        return this->tables.get(key);
};


int rfir::util::RegTable::encodeVectorAddress(std::vector<uint8_t>* value){
    return (int)value | 0x80000000;
};


std::vector<uint8_t>* rfir::util::RegTable::decodeVectorAddress(int value){
    return (std::vector<uint8_t>*)(value & 0x7FFFFFFF);
};
