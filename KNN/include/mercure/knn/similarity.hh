#ifndef MERCURE_SIMILARITY_HH__
#define MERCURE_SIMILARITY_HH__

#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <boost/container/flat_set.hpp>

namespace mercure {

template<typename T>
double euclidean(std::vector<T> const& xs, std::vector<T> const& ys, int missing = -1) {
  if (xs.empty() || ys.empty())
    return 0;

  double value = 0.0;
  size_t const n = std::min(xs.size(), ys.size());
  for (auto i = 0u; i < n; ++i) {
    if (i != missing) {
      value += (xs[i] - ys[i]) * (xs[i] - ys[i]);
    }
  }
  return sqrt(value);
}

template<typename T>
double covariance(std::vector<T> const& xs, std::vector<T> const& ys, int missing = -1) {
  double mean_x = 0, mean_y = 0, m = 0;
  size_t n = std::min(xs.size(), ys.size());
  for (auto i = 0u; i < n; ++i) {
    if (i == missing) continue;
    double const delta_x = (xs[i] - mean_x) / (i + 1);
    mean_x += delta_x;
    double const delta_y = (ys[i] - mean_y) / (i + 1);
    mean_y += delta_y;
    m += i * delta_x * delta_y - m / (i + 1);
  }
  if (missing != -1) --n;
  return n / (n - 1.0) * m;
}

} /* end namespace mercure */

#endif
