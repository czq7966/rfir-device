#include "reg-table.h"

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
            if (key >= this->charMinNum && key <= this->charMaxNum) {
                int len = strlen((const char*) value);
                memcpy(buf, &len, step);
                if (len > 0) {
                    len++;
                    memcpy(buf, &len, step);
                    buf = buf + step;
                    memcpy(buf, (void*)value, len);
                    buf = buf + len ;                
                } else {
                    buf = buf + step;
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

bool rfir::util::RegTable::decode(const char* buf, int size, std::list<int>& ids, bool createKey){
    const char* sbuf = buf;
    uint8_t step = 2;
    while(buf - sbuf < size) {
        uint16_t key;
        memcpy(&key, buf, step);
        buf = buf + step;
        if (key >= this->charMinNum && key <= this->charMaxNum) {
            uint16_t len;
            memcpy(&len, buf, step);
            buf = buf + step;            
            if (len > 0) {
                int value;
                if (this->tables.get(key, value)) {
                    memcpy((void*)value, buf, len);
                    ids.push_back(key);
                } else {
                    if (createKey) {
                        this->tables.add(key, (int)buf);
                        ids.push_back(key);
                    }
                }
                buf = buf + len;
            }
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

bool rfir::util::RegTable::decode(const char* buf, int size, bool createKey){
    std::list<int> ids;
    return this->decode(buf, size, ids, createKey);
};    

bool rfir::util::RegTable::merge(RegTable* regTable, std::list<int>& ids, bool createKey){
    auto map = regTable->tables.getMap();
    for (auto  it = map->begin(); it != map->end(); it++)
    {
        int key = it->first;            
        if (key >= this->charMinNum && key <= this->charMaxNum) {
            if (it->second) {
                int value = 0;
                if (this->tables.get(key, value) && value) {
                    strcpy((char*)value, (const char*)it->second); 
                    ids.push_back(key);                       
                } else {
                    if (createKey) {
                        this->tables.add(key, it->second);
                        ids.push_back(key);
                    }
                }
            }
        } else {
            this->tables.add(key, it->second);
            ids.push_back(key);
        } 
    }
    
    return true;
};
