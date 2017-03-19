/**
  \file   show_set.hh
  \brief  Overload the << operator for boost's flat_set.
 */
#ifndef SHOW_SET_HH__
#define SHOW_SET_HH__

#include <iostream>
#include <string>
#include <boost/variant.hpp>
#include "mercure/io/intersperse.hh"
#include "mercure/common.hh"

namespace std {

/**
  \brief Formats boost's flat_set as {elem0, elem1, elem2, elem3, ...}
 */
template<typename Key>
auto operator<<(ostream& os, mercure::set<Key> const& s)
                -> ostream& {
  os << '{' << mercure::intersperse(s.begin(), s.end()) << '}';
  return os;
}

/**
  \brief Formats boost's flat_set as {elem0, elem1, elem2, elem3, ...}
 */
template<typename Key>
auto operator<<(ostream& os, mercure::multiset<Key> const& s) -> ostream& {
  os << '{' << mercure::intersperse(s.begin(), s.end()) << '}';
  return os;
}

/**
  \brief Formats boost's flat_map as {key0 -> value0, key1 -> value1, ...}
 */
template<typename Key, typename Value>
auto operator<<(ostream& os, mercure::map<Key, Value> const& m)  -> ostream& {
  os << '{';
  if (!m.empty()) {
    auto const penultimate = --m.end();
    auto it = m.begin();
    for (; it != penultimate; ++it)
      os << it->first << " -> " << it->second << ", ";
    os << it->first << " -> " << it->second;
  }
  os << '}';
  return os;
}

/**
  \brief Formats boost's flat_map as {key0 -> value0, key1 -> value1, ...}
 */
template<typename Key, typename Value>
auto operator<<(ostream& os, mercure::multimap<Key, Value> const& m) -> ostream& {
  os << '{';
  if (!m.empty()) {
    auto const penultimate = --m.end();
    auto it = m.begin();
    for (; it != penultimate; ++it)
      os << it->first << " -> " << it->second << ", ";
    os << it->first << " -> " << it->second;
  }
  os << '}';
  return os;
}

} /* end namespace std */

#endif
