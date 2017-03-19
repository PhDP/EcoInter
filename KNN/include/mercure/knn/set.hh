#ifndef MERCURE_SET_HH__
#define MERCURE_SET_HH__

#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <unordered_set>
#include <vector>
#include "mercure/common.hh"

namespace mercure {

template<typename T>
auto set_union(set<T> const& xs, set<T> const& ys) -> std::vector<T> {
  auto u = std::vector<T>{};
  auto const xs_end = xs.end(), ys_end = ys.end();
  auto xs_it = xs.begin(), ys_it = ys.begin();

  while (xs_it != xs_end && ys_it != ys_end) {
    if (!(*xs_it < *ys_it) && !(*ys_it < *xs_it)) {
      u.push_back(*xs_it++);
      ++ys_it;
    } else if (*xs_it < *ys_it) {
      u.push_back(*xs_it++);
    } else { // *ys_it < *xs_it
      u.push_back(*ys_it++);
    }
  }
  while (xs_it != xs_end) u.push_back(*xs_it++);
  while (ys_it != ys_end) u.push_back(*ys_it++);
  return u;
}

template<typename T>
auto set_intersection(set<T> const& xs, set<T> const& ys) -> std::vector<T> {
  auto inter = std::vector<T>{};
  auto const xs_end = xs.end(), ys_end = ys.end();
  auto xs_it = xs.begin(), ys_it = ys.begin();

  while (xs_it != xs_end && ys_it != ys_end) {
    if (*xs_it < *ys_it) {
      ++xs_it;
    } else {
      if (!(*ys_it < *xs_it)) {
        inter.push_back(*xs_it++);
      }
      ++ys_it;
    }
  }
  return inter;
}

template<typename T>
auto set_difference(set<T> const& xs, set<T> const& ys) -> std::vector<T> {
  auto diff = std::vector<T>{};
  auto const xs_end = xs.end(), ys_end = ys.end();
  auto xs_it = xs.begin(), ys_it = ys.begin();

  while (xs_it != xs_end && ys_it != ys_end) {
    bool const x_lt_y = *xs_it < *ys_it;
    bool const y_lt_x = *ys_it < *xs_it;
    if (x_lt_y && !y_lt_x) {
      diff.push_back(*xs_it);
      ++xs_it;
    } else if (!x_lt_y) {
      ++ys_it;
      if (!y_lt_x)
        ++xs_it;
    }
  }
  // Insert remaining:
  while (xs_it != xs_end) diff.push_back(*xs_it++);
  return diff;
}

template<typename T>
auto set_union_size(set<T> const& xs, set<T> const& ys) -> size_t {
  size_t count = 0;
  auto const xs_end = xs.end(), ys_end = ys.end();
  auto xs_it = xs.begin(), ys_it = ys.begin();

  while (xs_it != xs_end && ys_it != ys_end) {
    if (!(*xs_it < *ys_it) && !(*ys_it < *xs_it)) {
      ++xs_it;
      ++ys_it;
    } else if (*xs_it < *ys_it) {
      ++xs_it;
    } else { // *ys_it < *xs_it
      ++ys_it;
    }
    ++count;
  }
  for (; xs_it != xs_end; ++xs_it) ++count;
  for (; ys_it != ys_end; ++ys_it) ++count;
  return count;
}

template<typename T>
auto set_intersection_size(set<T> const& xs, set<T> const& ys) -> size_t {
  size_t count = 0;
  auto const xs_end = xs.end(), ys_end = ys.end();
  auto xs_it = xs.begin(), ys_it = ys.begin();

  while (xs_it != xs_end && ys_it != ys_end) {
    if (*xs_it < *ys_it) {
      ++xs_it;
    } else {
      if (!(*ys_it < *xs_it)) {
        ++count;
        ++xs_it;
      }
      ++ys_it;
    }
  }
  return count;
}

} /* end namespace mercure */

#endif
