#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <array>
#include <ctime>
#include <functional>
#include "mercure/common.hh"
#include "mercure/confusion.hh"
#include "mercure/knn/tanimoto.hh"
#include "mercure/io/string_utils.hh"
#include "mercure/io/show_flat.hh"

class norm {
 public:
  using norm_type = std::function<double(double, double)>;

  norm(std::string const& name, norm_type const& t_norm, norm_type const& s_norm)
    noexcept : m_name(name), m_t_norm(t_norm), m_s_norm(s_norm) {
  }

  auto name() const noexcept -> std::string const& {
    return m_name;
  }

  auto t(double x, double y) const noexcept -> double {
    return m_t_norm(x, y);
  }

  auto s(double x, double y) const noexcept -> double {
    return m_s_norm(x, y);
  }

 private:
  std::string m_name;
  norm_type m_t_norm;
  norm_type m_s_norm;
};

auto const god_norm = norm("Gödel-Dummett",
                           [](double x, double y) { return std::min(x, y); },
                           [](double x, double y) { return std::max(x, y); });

auto const pro_norm = norm("Product",
                           [](double x, double y) { return x * y; },
                           [](double x, double y) { return x + y - x * y; });

auto const luk_norm = norm("Łukasiewicz",
                           [](double x, double y) { return std::max(0.0, x + y - 1.0); },
                           [](double x, double y) { return std::min(1.0, x + y); });

auto const norms = std::vector<norm>{god_norm, pro_norm, luk_norm};

using dataset = std::vector<mercure::set<uint32_t>>;

auto fuzzthat(uint32_t x, uint32_t a, dataset const& ds, norm const& n) -> double {
  auto ans = 0.0;
  for (uint32_t y = 0; y < ds.size(); ++y) {
    if (x == y) continue;
    double const similarity = mercure::tanimoto(ds[x], ds[y]);
    ans = n.s(ans, n.t(similarity, double(ds[y].find(a) != ds[y].end())));
  }
  return ans;
}

int main(int argc, char **argv) {
  auto const seed = (argc > 1)? atoi(argv[1]) : time(NULL);
  std::cout << "Seed = " << seed
            << "\n+------------------------------------+\n";

  // Read interactions:
  dataset const interactions = mercure::csv_into_sets("./data/mercure-interactions.csv");

  size_t count = 0;
  for (auto const &i : interactions) count += i.size();

  std::cout << "There are " << interactions.size() << " species in the data-set.\n";
  std::cout << "There are " << count << " interactions in the data-set.\n";

  // Random number generator & uniform distribution
  auto rng = std::mt19937_64(seed);
  auto unif = std::uniform_int_distribution<uint32_t>(0, interactions.size() - 1);

  dataset noninteractions;
  for (auto i = 0u; i < interactions.size(); ++i) noninteractions.push_back(mercure::set<uint32_t>{});

  assert(interactions.size() == noninteractions.size());

  size_t countnon = 0;
  while (countnon < count) {
    auto const predator = unif(rng);
    auto const prey = unif(rng);
    if (interactions[predator].find(prey) == interactions[predator].end()
        && noninteractions[predator].find(prey) == noninteractions[predator].end()) {
      noninteractions[predator].insert(prey);
      ++countnon;
    }
  }

  for (auto const n : norms) {
    auto c = mercure::confusion(); // Results

    for (auto i = 0u; i < interactions.size(); ++i) {
      for (auto const prey : interactions[i]) {
        auto const truth = fuzzthat(i, prey, interactions, n);
        if (truth > 0.5) {
          c.tp++;
        } else {
          c.fn++;
        }
      }
      for (auto const prey : noninteractions[i]) {
        auto const truth = fuzzthat(i, prey, interactions, n);
        if (truth > 0.5) {
          c.fp++;
        } else {
          c.tn++;
        }
      }
    }

    std::cout << "Results for " << n.name() << ":\n" << c << "\n\n";
  }
  return 0;
}
