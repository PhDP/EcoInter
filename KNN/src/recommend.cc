#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <array>
#include <ctime>
#include "mercure/knn/recommendations.hh"
#include "mercure/io/string_utils.hh"
#include "mercure/io/show_flat.hh"

int main(int argc, char **argv) {
  auto const seed = (argc > 1)? atoi(argv[1]) : time(NULL);
  std::cout << "Seed = " << seed
            << "\n********************************************************\n";

  // Random number generator & uniform distribution
  auto rng = std::mt19937_64(seed);
  auto unif = std::uniform_real_distribution<double>(0, 1);

  // Read interactions:
  auto inter = mercure::csv_into_sets("./data/mercure-interactions.csv");
  // Read traits
  auto traits = mercure::csv_into_sets("./data/mercure-traits.csv");
  // Number of species
  auto const n = inter.size();

  assert(inter.size() == traits.size());

  for (auto k = 1u; k <= 19; k += 2) {
    for (auto i = 0; i <= 5; ++i) {
      double const w = i * 0.2;

      auto successes = std::array<size_t, 10>{};
      auto count = 0u;

      // For each species with at least two preys, we'll randomly removed one of
      // their preys, recommend new preys based on the KNN algorithm, and check how
      // many attempts it takes to get back the missing species (max: 10 attempts).
      for (auto j = 0; j < 10; ++j) { // Repeat 10 times.
        for (auto species = 0u; species < n; ++species) {
          // The species must have at least 2 preys to attempt this:
          if (inter[species].size() < 2)
            continue;

          // Randomly remove a prey and store its value:
          size_t const n = inter[species].size();
          auto removed_it = inter[species].begin();
          size_t const advance_by = (size_t)(unif(rng) * n);
          assert(advance_by < n);
          std::advance(removed_it, advance_by);
          auto const removed_prey = *removed_it;
          inter[species].erase(removed_it);

          // Generate the recommendations with Mercure (we don't care about the count).
          auto recommendations = mercure::recommendations(k, inter, traits, species, w);

          size_t const maxr = std::min((unsigned int)recommendations.size(), 10u);
          for (auto r = 0u; r < maxr; ++r) {
            if (recommendations[r] == removed_prey) {
              successes[r]++;
              break;
            }
          }

          // Put back the original species' data in the dataset:
          assert(inter[species].size() == n - 1);
          inter[species].insert(removed_prey);
          assert(inter[species].size() == n);
          ++count;
        }
      }
      std::cout << "K = " << k << ", w = " << w << '\n'
                << "Count = " << count << '\n';
      for (auto s = 0u; s < 10; ++s) {
        auto const success = successes[s];
        std::cout << "  Success at the " << (s + 1) << "nth attempt = " << success
                  << " (" << (100.0 * success / count) << "%).\n";
      }
      std::cout << "********************************************************\n";
    }
  }
  return 0;
}
