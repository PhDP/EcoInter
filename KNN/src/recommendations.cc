#include <vector>
#include <cassert>
#include "mercure/knn/tanimoto.hh"
#include "mercure/knn/recommendations.hh"

namespace mercure {

auto recommendations(
    size_t k,
    std::vector<set<uint32_t>> const& data,
    std::vector<set<uint32_t>> const& traits,
    size_t species_idx,
    double w_t)
    -> std::vector<uint32_t> {
  assert(traits.size() == data.size());
  assert(species_idx >= 0 && species_idx < data.size());

  double const w_i = 1.0 - w_t;

  auto nearest = multimap<double, uint32_t>{};

  uint32_t i = 0;
  for (; i < data.size() && nearest.size() != k; ++i) {
    if (i == species_idx) continue;
    double const d = w_i * tanimoto_distance(data[species_idx], data[i])
                   + w_t * tanimoto_distance(traits[species_idx], traits[i]);

    if (d != 1.0)
      nearest.insert(std::pair<double, uint32_t>(d, i));
  }

  for (; i < data.size(); ++i) {
    if (i == species_idx) continue;
    double const d = w_i * tanimoto_distance(data[species_idx], data[i])
                   + w_t * tanimoto_distance(traits[species_idx], traits[i]);

    if (d < nearest.rbegin()->first)
      nearest.insert(std::pair<double, uint32_t>(d, i));
    if (nearest.size() > k)
      nearest.erase(nearest.equal_range(nearest.rbegin()->first).first);
  }

  auto nearest_vec = std::vector<uint32_t>{};
  nearest_vec.reserve(nearest.size());
  for (auto const& n : nearest)
    nearest_vec.push_back(n.second);

  assert(nearest_vec.size() == nearest.size()); // == k?

  // Map from species id to the number of time they are found in the K most similar species.
  auto elem_count = map<uint32_t, size_t>{};
  for (auto const& n : nearest_vec) {
    auto diff = set_difference(data[n], data[species_idx]);
    for (auto const& d : diff)
      elem_count[d]++;
  }

  // Reverse the map to a "count -> species id" multimap.
  auto count_elem = multimap<size_t, uint32_t>{};
  for (auto const& e : elem_count)
    count_elem.insert(std::pair<size_t, uint32_t>(e.second, e.first));

  // Use the count to get recommendations starting from those with the highest count:
  std::vector<uint32_t> rs;
  rs.reserve(count_elem.size());
  for (auto it = count_elem.rbegin(); it != count_elem.rend(); ++it)
    rs.push_back(it->second);
  return rs;
}

} /* end namespace mercure */
