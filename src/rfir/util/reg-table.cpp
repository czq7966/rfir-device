#include "reg-table.h"

bool rfir::util::RegTable::encode(char* buf, int& size, std::list<int>& ids){

    return 0;
}

bool rfir::util::RegTable::encode(char* buf, int& size){
    return 0;
};

bool rfir::util::RegTable::decode(const char* buf, int size, std::list<int>& ids){
        const char* pos = buf;
    while(pos - buf < size) {
        uint16_t* key = (uint16_t*)pos;    
        if (*key >= this->charMinNum && *key <= this->charMaxNum) {
            pos = pos + sizeof(*key);
            uint16_t* len = (uint16_t*)pos;
            pos = pos + sizeof(*len);
            int value;
            if (this->tables.get(*key, value)) {
                memcpy((void*)value, (void*)pos, *len);
                ids.push_back(*key);
            }         

        } else {
            pos = pos + sizeof(*key);
            uint16_t* value = (uint16_t*)pos;
            this->tables.add(*key, *value);
            ids.push_back(*key);
            pos = pos + sizeof(*value);
        }
    }
    return 0;
}

bool rfir::util::RegTable::decode(const char* buf, int size){
    std::list<int> ids;
    return this->decode(buf, size, ids);
};    

