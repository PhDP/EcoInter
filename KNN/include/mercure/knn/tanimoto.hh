#ifndef MERCURE_TANIMOTO_HH__
#define MERCURE_TANIMOTO_HH__

#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <unordered_set>
#include <vector>
#include "mercure/common.hh"
#include "mercure/knn/set.hh"

namespace mercure {

/**
 * \brief Tanimoto similarity on two vectors.
 */
template<typename T>
double tanimoto(std::vector<T> const& xs, std::vector<T> const& ys) {
  if (xs.empty() || ys.empty())
    return 0;

  size_t and_count = 0, or_count = 0;
  size_t const n = std::min(xs.size(), ys.size());
  for (auto i = 0u; i < n; ++i) {
    and_count += (xs[i] && ys[i]);
    or_count += (xs[i] || ys[i]);
  }
  return (double)and_count / or_count;
}

/**
 * \brief Computes the Tanimoto similarity index between two sets (size of the
 *        intersection divided by the size of the union of the sets).
 */
template<typename T>
double tanimoto(set<T> const& xs, set<T> const& ys) {
  if (xs.empty() || ys.empty())
    return 0.0;
  size_t const i = set_intersection_size(xs, ys);
  return (double)i / (xs.size() + ys.size() - i);
}

/**
 * \brief Computes the Tanimoto similarity index between two sets (size of the
 *        intersection divided by the size of the union of the sets).
 */
template<typename T>
double tanimoto(std::unordered_set<T> const& xs, std::unordered_set<T> const& ys) {
  if (xs.size() > ys.size())
    return tanimoto(ys, xs);
  if (xs.empty())
    return 0;

  size_t const i = std::accumulate(xs.begin(), xs.end(), 0, [=](T const& x, size_t count) {
    return count + (ys.find(x) != ys.end());
  });

  return (double)i / (xs.size() + ys.size() - i);
}

template<typename T>
double tanimoto_distance(set<T> const& xs, set<T> const& ys) {
  return 1.0 - tanimoto(xs, ys);
}

template<typename T>
double tanimoto_distance(std::vector<T> const& xs, std::vector<T> const& ys) {
  return 1.0 - tanimoto(xs, ys);
}

template<typename T>
double tanimoto_distance(std::unordered_set<T> const& xs, std::unordered_set<T> const& ys) {
  return 1.0 - tanimoto(xs, ys);
}

} /* end namespace mercure */

#endif
