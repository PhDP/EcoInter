#include "catch.hpp"
#include <iostream>
#include <vector>
#include "mercure/common.hh"
#include "mercure/knn/set.hh"
#include "mercure/io/show_flat.hh"

TEST_CASE("Intersection", "[set operations]") {

  auto const int_set0 = mercure::set<int>{};
  auto const int_set1 = mercure::set<int>{0, 2, 4, 6, 8};
  auto const int_set2 = mercure::set<int>{1, 3, 5, 7, 9};
  auto const int_set3 = mercure::set<int>{0, 1, 2, 4, 5};
  auto const int_set4 = mercure::set<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
  auto const int_set5 = mercure::set<int>{-1, 4, 9, 14};
  auto const sets = std::vector<mercure::set<int>> {
    int_set0, int_set1, int_set2, int_set3, int_set4, int_set5
  };

  SECTION("Intersection is commutative") {
    for (auto const& x : sets) {
      for (auto const& y : sets) {
        REQUIRE(mercure::set_intersection_size(x, y) == mercure::set_intersection_size(y, x));
      }
    }
  }

  SECTION("Size") {
    REQUIRE(mercure::set_intersection_size(int_set0, int_set1) == 0);
    REQUIRE(mercure::set_intersection_size(int_set1, int_set2) == 0);
    REQUIRE(mercure::set_intersection_size(int_set1, int_set3) == 3);
    REQUIRE(mercure::set_intersection_size(int_set3, int_set4) == 5);
    REQUIRE(mercure::set_intersection_size(int_set4, int_set5) == 3);
  }

  SECTION("New set") {
    auto const i0 = std::vector<int>(int_set0.begin(), int_set0.end());
    REQUIRE(mercure::set_intersection(int_set1, int_set2) == i0);
    auto const ns0 = std::vector<int>{1, 5};
    REQUIRE(mercure::set_intersection(int_set2, int_set3) == ns0);
    auto const ns1 = std::vector<int>{4, 9, 14};
    REQUIRE(mercure::set_intersection(int_set4, int_set5) == ns1);
  }
}

TEST_CASE("Union", "[set operations]") {

  auto const c_set0 = mercure::set<char>{};
  auto const c_set1 = mercure::set<char>{'a', 'e', 'i', 'o', 'u', 'y'};
  auto const c_set2 = mercure::set<char>{'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
  auto const c_set3 = mercure::set<char>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  auto const sets = std::vector<mercure::set<char>> {
    c_set0, c_set1, c_set2, c_set3
  };

  SECTION("Union is commutative") {
    for (auto const& x : sets) {
      for (auto const& y : sets) {
        REQUIRE(mercure::set_union_size(x, y) == mercure::set_union_size(y, x));
      }
    }
  }

  SECTION("Size") {
    REQUIRE(mercure::set_union_size(c_set0, c_set1) == 6);
    REQUIRE(mercure::set_union_size(c_set0, c_set2) == 21);
    REQUIRE(mercure::set_union_size(c_set0, c_set3) == 26);
    REQUIRE(mercure::set_union_size(c_set1, c_set2) == 26);
    REQUIRE(mercure::set_union_size(c_set3, c_set1) == 26);
    REQUIRE(mercure::set_union_size(c_set2, c_set3) == 26);
  }

  SECTION("New set") {
    auto const c_vec1 = std::vector<char>{'a', 'e', 'i', 'o', 'u', 'y'};
    auto const c_vec3 = std::vector<char>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    REQUIRE(mercure::set_union(c_set0, c_set1) == c_vec1);
    REQUIRE(mercure::set_union(c_set0, c_set3) == c_vec3);
    REQUIRE(mercure::set_union(c_set1, c_set2) == c_vec3);
  }
}

TEST_CASE("Set difference", "[set operations]") {

  auto const int_set0 = mercure::set<int>{};
  auto const int_set1 = mercure::set<int>{0, 2, 4, 6, 8};
  auto const int_set2 = mercure::set<int>{1, 3, 5, 7, 9};
  auto const int_set3 = mercure::set<int>{0, 1, 2, 4, 5};
  auto const int_set4 = mercure::set<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
  auto const int_set5 = mercure::set<int>{-1, 4, 9, 14};

  SECTION("New set") {
    auto const new_int_set2 = std::vector<int>{3, 7, 9};
    REQUIRE(mercure::set_difference(int_set2, int_set3) == new_int_set2);

    auto const new_int_set3 = std::vector<int>{0, 2, 4};
    REQUIRE(mercure::set_difference(int_set3, int_set2) == new_int_set3);

    auto const empty_vec = std::vector<int>{};
    auto const int_vec1 = std::vector<int>{0, 2, 4, 6, 8};
    auto const int_vec2 = std::vector<int>{1, 3, 5, 7, 9};

    REQUIRE(mercure::set_difference(int_set0, int_set2) == empty_vec);
    REQUIRE(mercure::set_difference(int_set1, int_set2) == int_vec1);
    REQUIRE(mercure::set_difference(int_set1, int_set1) == empty_vec);
    REQUIRE(mercure::set_difference(int_set2, int_set1) == int_vec2);
    REQUIRE(mercure::set_difference(int_set3, int_set4) == empty_vec);

    auto const new_int_set4 = std::vector<int>{0, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 13};
    REQUIRE(mercure::set_difference(int_set4, int_set5) == new_int_set4);

    auto const minus1 = std::vector<int>{-1};
    REQUIRE(mercure::set_difference(int_set5, int_set4) == minus1);
  }
}
