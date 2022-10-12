
// #include "map-factory.h"


// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::get(KeyType key, ValueType& value) {
//     auto it = m_map.find(key);
//     if (it != m_map.end()) {
//         value = it->second;
//         return true;
//     }

//     return false;
// }

// template <typename KeyType, typename ValueType>
// ValueType rfir::util::MapFactory<KeyType, ValueType>::get(KeyType key) {
//     ValueType value;
//     if (get(key, value))
//         return value;
//     return 0;
// }

// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::getBegin(KeyType& key, ValueType& value, bool erase) {
//     auto it = m_map.begin();
//     if (it != m_map.end()) {
//         key = it->first;
//         value = it->second;
//         if (erase)
//             m_map.erase(it);
//         return true;
//     };
//     return false;
// }

// template <typename KeyType, typename ValueType>
// void rfir::util::MapFactory<KeyType, ValueType>::add(KeyType key, ValueType value) {
//     m_map[key] = value;
// }

// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::remove(KeyType key) {
//     auto it = m_map.find(key);
//     if (it != m_map.end()) {
//         m_map.erase(key);
//         return true;
//     } else {
//         return false;
//     }
// }

// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::remove(KeyType key, ValueType& value) {
//     bool result = get(key, value);
//     m_map.erase(key);
//     return result;
// }

// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::removeBegin(KeyType& key, ValueType& value) {
//     return this->getBegin(key, value, true);
// }

// template <typename KeyType, typename ValueType>
// void rfir::util::MapFactory<KeyType, ValueType>::clear() {
//     m_map.clear();

// }

// template <typename KeyType, typename ValueType>
// std::map<KeyType, ValueType>* rfir::util::MapFactory<KeyType, ValueType>::getMap() {
//     return &m_map;
// }


// template <typename KeyType, typename ValueType>
// int rfir::util::MapFactory<KeyType, ValueType>::getSize() {
//     return m_map.size();
// }

// template <typename KeyType, typename ValueType>
// std::list<KeyType> rfir::util::MapFactory<KeyType, ValueType>::getKeys() {
//     std::list<KeyType> keys;
//     for (auto it = m_map.begin(); it != m_map.end(); ++it) {
//         keys.push_back(it->first);
//     }
//     return keys;
// }

// template <typename KeyType, typename ValueType>
// std::list<ValueType> rfir::util::MapFactory<KeyType, ValueType>::getValues() {
//     std::list<ValueType> values;
//     for (auto it = m_map.begin(); it != m_map.end(); ++it) {
//         values.push_back(it->second);
//     }
//     return values;
// }

// template <typename KeyType, typename ValueType>
// bool rfir::util::MapFactory<KeyType, ValueType>::existValue(ValueType value) {
//     for (auto it = m_map.begin(); it != m_map.end(); ++it) {
//         if (it->second == value)
//             return true;
//     }
//     return false;
// }

