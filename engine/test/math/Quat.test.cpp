#include "math/algebras.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>

Catch::Approx operator""_a(long double val) {
  return Catch::Approx(val).scale(1);
}
Catch::Approx operator""_a(unsigned long long val) {
  return Catch::Approx(val).scale(1);
}

TEST_CASE("Quat equality and inequality", "[quat]") {
  Quat a{1, 2, 3, 4};
  Quat b{1, 2, 3, 4};
  Quat c{4, 3, 2, 1};
  REQUIRE(a == b);
  REQUIRE_FALSE(a == c);
  REQUIRE(a != c);
  REQUIRE_FALSE(a != b);
}

TEST_CASE("Quat identity constant", "[quat]") {
  REQUIRE(Quat::IDENTITY == Quat{1, 0, 0, 0});
}

TEST_CASE("Quat fromAxisAngle 360 degrees returns identity", "[quat]") {
  Vec3 axis{0, 1, 0};
  float radians = 2 * M_PI;
  Quat q = Quat::fromAxisAngle(axis, radians);
  REQUIRE(q.w == -1_a);
  REQUIRE(q.x == 0_a);
  REQUIRE(q.y == 0_a);
  REQUIRE(q.z == 0_a);
}

TEST_CASE("Quats for axis of different lenghts", "[quat]") {
  Vec3 axis1{0, 1, 0};
  Vec3 axis2{0, 2, 0};
  float radians = M_PI_2;
  Quat q1 = Quat::fromAxisAngle(axis1, radians);
  Quat q2 = Quat::fromAxisAngle(axis2, radians);
  REQUIRE(q1 == q2);
}

TEST_CASE("Quat fromEuler", "[quat]") {
  // Zero rotation
  Quat q = Quat::fromEuler(0, 0, 0);
  REQUIRE(q == Quat::IDENTITY);
  // 180 degree rotation around X
  Quat qx = Quat::fromEuler(M_PI, 0, 0);
  REQUIRE(qx.w == 0_a);
  REQUIRE(qx.x == 1_a);
  REQUIRE(qx.y == 0_a);
  REQUIRE(qx.z == 0_a);
  // 180 degree rotation around Y
  Quat qy = Quat::fromEuler(0, M_PI, 0);
  REQUIRE(qy.w == 0_a);
  REQUIRE(qy.x == 0_a);
  REQUIRE(qy.y == 1_a);
  REQUIRE(qy.z == 0_a);
  // 180 degree rotation around Z
  Quat qz = Quat::fromEuler(0, 0, M_PI);
  REQUIRE(qz.w == 0_a);
  REQUIRE(qz.x == 0_a);
  REQUIRE(qz.y == 0_a);
  REQUIRE(qz.z == 1_a);
}

TEST_CASE("Quat normalized", "[quat]") {
  REQUIRE(Quat{2, 0, 0, 0}.normalized() == Quat::IDENTITY);
  REQUIRE_THAT(
      Quat(3, 5, 4, 7).normalized(),
      Catch::Matchers::Predicate<Quat>(
          [](const Quat &q)
          {
            return q.w == 0.30151134_a && q.x == 0.50251891_a && q.y == 0.40201513_a && q.z == 0.70352647_a;
          },
          ""
      )
  );
}

TEST_CASE("Quat inverse", "[quat]") {
  Quat q{1, 2, 3, 4};
  Quat inv = q.inverse();
  float norm = 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4;
  REQUIRE(inv == Quat{1.0f / norm, -2.0f / norm, -3.0f / norm, -4.0f / norm});
}

TEST_CASE("Quat arithmetic operators", "[quat]") {
  Quat a{1, 2, 3, 4};
  Quat b{5, 6, 7, 8};
  REQUIRE(a + b == Quat{6, 8, 10, 12});
  REQUIRE(a - b == Quat{-4, -4, -4, -4});
  REQUIRE(a * 2.0f == Quat{2, 4, 6, 8});
  REQUIRE(a / 2.0f == Quat{0.5f, 1.0f, 1.5f, 2.0f});
  REQUIRE(
      a * b ==
      Quat{
          1 * 5 - 2 * 6 - 3 * 7 - 4 * 8,
          1 * 6 + 2 * 5 + 3 * 8 - 4 * 7,
          1 * 7 - 2 * 8 + 3 * 5 + 4 * 6,
          1 * 8 + 2 * 7 - 3 * 6 + 4 * 5
      }
  );
  // a / b == a * b.inverse()
  Quat binv = b.inverse();
  REQUIRE(a / b == a * binv);
}
