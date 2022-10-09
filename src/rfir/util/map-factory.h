

#ifndef _RFIR_UTIL_MAP_FACTORY_H__
#define _RFIR_UTIL_MAP_FACTORY_H__

#include <map>
#include <list>


namespace rfir {
    namespace util {
        template <typename KeyType, typename ValueType>
        class MapFactory {
            std::map<KeyType, ValueType> m_map;
        public:
            bool get(KeyType key, ValueType& value){
                auto it = m_map.find(key);
                if (it != m_map.end()) {
                    value = it->second;
                    return true;
                }

                return false;                
            };

            ValueType get(KeyType key){
                ValueType value;
                if (get(key, value))
                    return value;
                return 0;                
            };

            bool getBegin(KeyType& key, ValueType& value, bool erase = false){
                auto it = m_map.begin();
                if (it != m_map.end()) {
                    key = it->first;
                    value = it->second;
                    if (erase)
                        m_map.erase(it);
                    return true;
                };
                return false;                
            };
            void add(KeyType key, ValueType value){
                m_map[key] = value;                
            };

            bool remove(KeyType key){
                auto it = m_map.find(key);
                if (it != m_map.end()) {
                    m_map.erase(key);
                    return true;
                } else {
                    return false;
                }                
            };

            bool remove(KeyType key, ValueType& value){ 
                bool result = get(key, value);
                m_map.erase(key);
                return result;                
            };

            bool removeBegin(KeyType& key, ValueType& value){
                return this->getBegin(key, value, true);                
            };

            void clear(){
                m_map.clear();                
            };

            std::map<KeyType, ValueType>* getMap(){
                return &m_map;                
            };

            int getSize(){
                return m_map.size();                
            };

            int getKeys(std::list<KeyType>& keys){
                for (auto it = m_map.begin(); it != m_map.end(); ++it) {
                    keys.push_back(it->first);
                }
                return m_map.size();                
            };

            int getValues(std::list<ValueType>& values){
                for (auto it = m_map.begin(); it != m_map.end(); ++it) {
                    values.push_back(it->second);
                }
                return m_map.size();                
            };

            bool getKeyByValue(ValueType value, KeyType& key) {
                for (auto it = m_map.begin(); it != m_map.end(); ++it) {
                    if (it->second == value) {
                        key = it->first;
                        return true;
                    }
                }
                return false;
            }

            bool existValue(ValueType value){
                for (auto it = m_map.begin(); it != m_map.end(); ++it) {
                    if (it->second == value)
                        return true;
                }
                return false;                
            };
        };
    }
}


#endif //_CMDS_COMMON_MAP_FACTORY_H__
