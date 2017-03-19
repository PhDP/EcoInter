#ifndef MERCURE_IMPUTATION_HH__
#define MERCURE_IMPUTATION_HH__

#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include "mercure/knn/symat.hh"
#include "mercure/common.hh"

namespace mercure {

inline auto distance_matrix(
    std::vector<std::vector<uint32_t>> const& inters,
    std::vector<std::vector<double>> const& ctraits,
    std::vector<std::vector<uint32_t>> const& traits,
    double w_m = 1.0, double w_p = 1.0, double w_t = 1.0, double w_i = 1.0)
    -> symat<double> {
  auto const n = inters.size();
  auto m = symat<double>(n);
  for (auto i = 0u; i < m.size(); ++i) {
    auto const x = m.row_idx(i);
    auto const y = m.col_idx(i);

    double distance = 0.0;
    distance += (w_m * ctraits[x][0] - w_m * ctraits[y][0]) * (w_m * ctraits[x][0] - w_m * ctraits[y][0]);
    distance += (w_p * ctraits[x][1] - w_p * ctraits[y][1]) * (w_p * ctraits[x][1] - w_p * ctraits[y][1]);
    distance += (w_p * ctraits[x][2] - w_p * ctraits[y][2]) * (w_p * ctraits[x][2] - w_p * ctraits[y][2]);

    for (auto t = 0u; t < traits[x].size(); ++t)
      distance += (w_t * traits[x][t] - w_t * traits[y][t]) * (w_t * traits[x][t] - w_t * traits[y][t]);

    for (auto j = 0u; j < inters[x].size(); ++j)
      distance += (w_i * inters[x][j] - w_i * inters[y][j]) * (w_i * inters[x][j] - w_i * inters[y][j]);

    m(i) = sqrt(distance);
  }
  return m;
}

template<typename T>
auto imputation(size_t k, std::vector<std::vector<T>> const& inters,
                size_t conso, size_t resso, symat<double> const& d) -> T {
  // Find k nearest:
  auto const n = inters.size();
  auto nearest = multimap<double, size_t>{};

  auto i = 0u;
  for (; i < n && nearest.size() != k; ++i) {
    if (i == conso) continue;
    nearest.insert(std::pair<double, size_t>(d(i, conso), i));
  }

  for (; i < n; ++i) {
    if (i == conso) continue;
    double const distance = d(i, conso);
    if (distance < nearest.rbegin()->first)
      nearest.insert(std::pair<double, size_t>(distance, i));
    if (nearest.size() > k)
      nearest.erase(nearest.equal_range(nearest.rbegin()->first).first);
  }

  // Vote true:
  size_t trues = 0;

  for (auto const& near : nearest)
    trues += inters[near.second][resso] == 1;

  return trues > k / 2;
}

} /* end namespace mercure */

#endif
