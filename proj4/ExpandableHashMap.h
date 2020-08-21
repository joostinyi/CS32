// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#include <functional>
#include <list>
#include <vector>
using namespace std;

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
    
private:
    unsigned int getBucketNumber(const KeyType& key, const int size) const
    {
        unsigned int hasher(const KeyType& k); // prototype
        return hasher(key) % size;
    }
    int numPairs;
    vector<list<pair<KeyType, ValueType>>> buckets;
    double maxLoadFactor;
    void rehash();
    
};
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
: numPairs(0), buckets(8), maxLoadFactor(maximumLoadFactor) {}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    buckets.clear();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    buckets.clear();
    numPairs = 0;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return numPairs;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if(find(key) == nullptr)
    {
        buckets[getBucketNumber(key, buckets.size())].push_back(pair<KeyType, ValueType>(key,value));
        numPairs++;
    }
    else
        *find(key) = value;
    if(numPairs/buckets.size() > maxLoadFactor)
        rehash();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::rehash()
{
    vector<list<pair<KeyType, ValueType>>> newBuckets(buckets.size() * 2);
    for(list<pair<KeyType, ValueType>> li : buckets)
        for(pair<KeyType, ValueType> p : li)
            newBuckets[getBucketNumber(p.first, newBuckets.size())].push_back(pair<KeyType, ValueType>(p.first, p.second));
    buckets.clear();
    buckets = newBuckets;
}

//template<typename KeyType, typename ValueType>
//const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
//{
//    auto temp = buckets[getBucketNumber(key, buckets.size())];
//    for(auto it = temp.begin(); it != temp.end(); it++)
//        if((*it).first == key)
//            return &(*it).second;
//    return nullptr;
//}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    auto it = buckets[getBucketNumber(key, buckets.size())].begin();
    for(; it != buckets[getBucketNumber(key, buckets.size())].end(); it++)
        if((*it).first == key)
            return &(*it).second;
    return nullptr;
}

