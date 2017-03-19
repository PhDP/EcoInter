#include "catch.hpp"
#include <iostream>
#include <vector>
#include <boost/container/flat_set.hpp>
#include "mercure/knn/symat.hh"

TEST_CASE("Symmetric matrix", "[symat]") {

  SECTION("Dimensions") {
    auto m1 = mercure::symat<double>(1);
    REQUIRE(m1.size() == 0);
    REQUIRE(m1.dim() == 1);

    auto m2 = mercure::symat<double>(2);
    REQUIRE(m2.size() == 1);
    REQUIRE(m2.dim() == 2);

    auto m3 = mercure::symat<double>(3);
    REQUIRE(m3.size() == 3);
    REQUIRE(m3.dim() == 3);

    auto m4 = mercure::symat<double>(4);
    REQUIRE(m4.size() == 6);
    REQUIRE(m4.dim() == 4);

    auto m5 = mercure::symat<double>(5);
    REQUIRE(m5.size() == 10);
    REQUIRE(m5.dim() == 5);

    auto m6 = mercure::symat<double>(6);
    REQUIRE(m6.size() == 15);
    REQUIRE(m6.dim() == 6);

    auto m7 = mercure::symat<double>(7);
    REQUIRE(m7.size() == 21);
    REQUIRE(m7.dim() == 7);
  }

  SECTION("operator(idx) and operator(row, col)") {
    auto m = mercure::symat<double>(6);
    for (auto i = 0u; i < m.size(); ++i)
      m(i) = (double)i;

    REQUIRE(m.size() == 15);

    REQUIRE(m(0) == Approx(0.0));
    REQUIRE(m(5) == Approx(5.0));
    REQUIRE(m(14) == Approx(14.0));

    REQUIRE(m(0, 0) == Approx(0.0));
    REQUIRE(m(1, 0) == Approx(0.0));
    REQUIRE(m(2, 0) == Approx(1.0));
    REQUIRE(m(3, 0) == Approx(2.0));
    REQUIRE(m(4, 0) == Approx(3.0));
    REQUIRE(m(5, 0) == Approx(4.0));
    REQUIRE(m(2, 1) == Approx(5.0));
    REQUIRE(m(4, 2) == Approx(10.0));

    REQUIRE(m(0, 0) == 0.0);
    REQUIRE(m(1, 1) == 0.0);
    REQUIRE(m(4, 4) == 0.0);
    REQUIRE(m(5, 5) == 0.0);

    REQUIRE(m(2, 4) == m(4, 2));

    REQUIRE(m.col_idx(0) == 0);
    REQUIRE(m.col_idx(1) == 0);
    REQUIRE(m.col_idx(4) == 0);
    REQUIRE(m.col_idx(5) == 1);
    REQUIRE(m.col_idx(8) == 1);
    REQUIRE(m.col_idx(9) == 2);
    REQUIRE(m.col_idx(10) == 2);
    REQUIRE(m.col_idx(11) == 2);
    REQUIRE(m.col_idx(12) == 3);
    REQUIRE(m.col_idx(13) == 3);
    REQUIRE(m.col_idx(14) == 4);

    REQUIRE(m.row_idx(0) == 1);
    REQUIRE(m.row_idx(1) == 2);
    REQUIRE(m.row_idx(4) == 5);
    REQUIRE(m.row_idx(5) == 2);
    REQUIRE(m.row_idx(8) == 5);
    REQUIRE(m.row_idx(9) == 3);
    REQUIRE(m.row_idx(10) == 4);
    REQUIRE(m.row_idx(11) == 5);
    REQUIRE(m.row_idx(12) == 4);
    REQUIRE(m.row_idx(13) == 5);
    REQUIRE(m.row_idx(14) == 5);
  }
}

