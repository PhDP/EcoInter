#include "catch.hpp"
#include <iostream>
#include <vector>
#include "mercure/common.hh"
#include "mercure/knn/tanimoto.hh"
#include "mercure/io/show_flat.hh"

TEST_CASE("Tanimoto on flat sets", "[tanimoto]") {
  auto const int_set0 = mercure::set<int>{};
  auto const int_set1 = mercure::set<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto const int_set2 = mercure::set<int>{0, 2, 4, 6, 8};
  auto const int_set3 = mercure::set<int>{1, 3, 5, 7, 9};
  auto const int_set4 = mercure::set<int>{0, 1, 2, 3};
  auto const int_set5 = mercure::set<int>{2, 3, 4, 5};
  auto const int_set6 = mercure::set<int>{0, 7, 8, 9};
  auto const int_set7 = mercure::set<int>{1, 3, 4, 8, 9, 10, 11, 12, 13, 14, 15};
  auto const sets = std::vector<mercure::set<int>> {
    int_set0, int_set1, int_set2, int_set3, int_set4, int_set5, int_set6, int_set7
  };

  SECTION("Tanimoto is commutative") {
    for (auto const& x : sets) {
      for (auto const& y : sets) {
        REQUIRE(mercure::tanimoto(x, y) == mercure::tanimoto(y, x));
      }
    }
  }

  SECTION("Tanimoto index") {
    REQUIRE(mercure::tanimoto(int_set0, int_set2) == Approx(0.0));
    REQUIRE(mercure::tanimoto(int_set1, int_set2) == Approx(1.0 / 2.0));
    REQUIRE(mercure::tanimoto(int_set7, int_set1) == Approx(5.0 / 16.0));
    REQUIRE(mercure::tanimoto(int_set4, int_set5) == Approx(2.0 / 6.0));
  }

  SECTION("Tanimoto distance") {
    REQUIRE(mercure::tanimoto_distance(int_set0, int_set0) == Approx(1.0));
    REQUIRE(mercure::tanimoto_distance(int_set5, int_set4) == Approx(4.0 / 6.0));
  }
}

TEST_CASE("Tanimot on vectors", "[tanimoto]") {
  // intersection = 3, union = 5.
  auto const bv0 = std::vector<bool>{true, true, true,  false, true, false};
  auto const bv1 = std::vector<bool>{true, true, false, false, true, true, false, false, true};

  // intersection = 2, union = 5.
  auto const iv0 = std::vector<int>{0, 0, 2, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1};
  auto const iv1 = std::vector<int>{0, 1, 1, 0, 0, 0, 0, 1, 1};

  SECTION("Tanimoto index") {
    REQUIRE(mercure::tanimoto(bv0, bv1) == Approx(3.0 / 5.0));
    REQUIRE(mercure::tanimoto(bv1, bv0) == mercure::tanimoto(bv0, bv1));
  }

  SECTION("Tanimoto distance") {
    REQUIRE(mercure::tanimoto_distance(iv0, iv1) == Approx(3.0 / 5.0));
    REQUIRE(mercure::tanimoto_distance(iv1, iv0) == mercure::tanimoto_distance(iv0, iv1));
  }
}
