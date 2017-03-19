#ifndef MERCURE_COMMON_HH__
#define MERCURE_COMMON_HH__

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <functional>

namespace mercure {

template<typename Key, typename Compare = std::less<Key>>
using set = boost::container::flat_set<Key, Compare>;

template<typename Key, typename Compare = std::less<Key>>
using multiset = boost::container::flat_multiset<Key, Compare>;

template<typename Key, typename Value>
using map = boost::container::flat_map<Key, Value>;

template<typename Key, typename Value>
using multimap = boost::container::flat_multimap<Key, Value>;

} /* end namespace mercure */

#endif
