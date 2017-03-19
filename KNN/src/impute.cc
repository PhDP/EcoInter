#include <iostream>
#include <iterator>
#include <random>
#include <array>
#include <ctime>
#include "mercure/knn/imputation.hh"
#include "mercure/io/string_utils.hh"
#include "mercure/io/show_flat.hh"
#include "mercure/common.hh"

// phdp@xps15:~/Dropbox/work/eco/Mercure/examples$ ./imputation 1 5 0 1 1
// Parameters:
// ***************
//   K = 1
//   w_m = 5
//   w_p = 0
//   w_t = 1
//   w_i = 1
//   Seed = 0
//
// Accuracy: 0.9804.
// Accuracy1: 0.672554.
// Accuracy0: 0.993863.
// TSS: 0.666417.

int main(int argc, char **argv) {
  auto const k = (argc > 1)? atoi(argv[1]) : 5;
  auto const w_m = (argc > 2)? atof(argv[2]) : 1.0;
  auto const w_p = (argc > 3)? atof(argv[3]) : 1.0;
  auto const w_t = (argc > 4)? atof(argv[4]) : 1.0;
  auto const w_i = (argc > 5)? atof(argv[5]) : 1.0;

  auto const seed = (argc > 3)? atoi(argv[6]) : time(NULL);
  std::cout
    << "Parameters:\n***************\n"
    << "  K = " << k << '\n'
    << "  w_m = " << w_m << '\n'
    << "  w_p = " << w_p << '\n'
    << "  w_t = " << w_t << '\n'
    << "  w_i = " << w_i << '\n'
    << "  Seed = " << seed << "\n\n";

  // Read interactions:
  auto inter = mercure::csv_sets_into_vectors("./data/mercure-interactions.csv", 909);
  // Read traits
  auto traits = mercure::csv_sets_into_vectors("./data/mercure-traits.csv", 28);
  // Continuous traits:
  auto ctraits = mercure::csv_into_vectors("./data/mercure-real.csv");

  // Number of species
  auto const n = inter.size();

  auto m = mercure::distance_matrix(inter, ctraits, traits, w_m, w_p, w_t, w_i);

  auto rng = std::mt19937_64(seed);
  auto unif = std::uniform_int_distribution<size_t>(0, n - 1);

  size_t tt = 0, tf = 0, ft = 0, ff = 0;
  for (auto idx = 0; idx < 50000; ++idx) {
    auto const conso = unif(rng);
    auto const resso = unif(rng);
    auto const observed = inter[conso][resso];
    auto const predicted = mercure::imputation(k, inter, conso, resso, m);

    if (observed) {
      if (predicted) ++tt; else ++ff;
    } else {
      if (predicted) ++ft; else ++tf;
    }
  }

  std::cout
    << "Accuracy: " << ((tt + tf) / ((double)(tt + tf + ft + ff))) << ".\n"
    << "Accuracy1: " << (tt / ((double)(tt + ff))) << ".\n"
    << "Accuracy0: " << (tf / ((double)(tf + ft))) << ".\n"
    << "TSS: " <<     (((double)((tt * tf) - (ft * ff))) / ((tt + ff) * (ft + tf))) << ".\n"
    << "tt: " << tt
    << ", tf: " << tf
    << ", ft: " << ft
    << ", ff: " << ff << ".\n";

  return 0;
}
