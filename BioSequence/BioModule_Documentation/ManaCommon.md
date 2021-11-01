# ManaCommon

## Methods
```c++
/**
*Swap K to V and V to K
*@param p std::pair<const K, V> pair
*@return pair<const V, K>
*/
template<typename K, typename V>
std::map<K, V> convert_map(const boost::unordered_map<K, V> &in);
/**
*Swap K to V and V to K
*@param p std::pair<const K, V> pair
*@return pair<const V, K>
*/
template<typename K, typename V>
std::pair<const V, K> flip_pair(const std::pair<const K, V>& p);
/**
*invert map
*@param in map<K, V>
*@return multimap<V, K> 
*/
template<typename K, typename V>
std::multimap<V, K> invert_map(const std::map<K, V>& in);
/**
*Sort by value
*@param key_map std::map<K, V>
*@return multimap<V, K>
*/
template<typename K, typename V>
std::multimap<V, K> sortByValue(const std::map<K, V> &key_map);
/**
* Sort by value
*@param unordered unordered_map<K, V>
*@return multimap<V, K>
*/
template<typename K, typename V>
std::multimap<V, K> sortByValue(const boost::unordered_map<K, V> &unordered);
/**
*Get size
*@param p pair<const K, const V>
*@return std::pair<int, const K>
*/
template<typename K, typename V>
std::pair<int, const K> get_size(const std::pair<const K, const V> &p);
/**
* size map
*@param p map<K, V>
*@return std::multimap<int, K>
*/
template<typename K, typename V>
std::multimap<int, K> size_map(const std::map<K, V> &in);
/**
*size map
*@param in boost::unordered_map<const K, V>
*@return multimap<int, K>
*/
template<typename K, typename V>
std::multimap<int, K> size_map(const boost::unordered_map<const K, V>& in);
/**
*Swap K to V and V to K
*@param key_map std::map<K, V>
*@return std::multimap<int, K>
*/
template<typename K, typename V>
std::multimap<int, K> sortBySize(std::map<K, V> &key_map) ;
/**
* Sort By Size
*@param key_map boost::unordered_map<K, V>
*@return multimap<int, K>
*/
template<typename K, typename V>
std::multimap<int, K> sortBySize(boost::unordered_map<K, V> &key_map) ;
/**
* Extract values
*@param in std::multimap<K, V>
*@return vector<V>
*/
template<typename K, typename V>
std::vector<V> extractValues(std::multimap<K, V> &in);
/**
* Extract values
*@param p std::map<K, V> in
*@return vector<V>
*/
template<typename K, typename V>
std::vector<V> extractValues(std::map<K, V> &in);
/**
*Get min vaule between a and b
*@param a K 
*@param b K 
*@return K
*/
template<typename K> K min(K a, K b) ;
/**
*Swap objects
*@param a T *
*@param b T *
*@return void
*/
template <typename T> void swap( T &a, T &b );
/**
*Swap Pointers
*@param a T *
*@param b T *
*@return void
*/
template <typename T> void swapPtr( T *a, T *b );
```