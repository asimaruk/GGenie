#include "math/algebras.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>

using namespace GGenie;

auto Approximately(const Quat &expected, float epsilon = std::numeric_limits<float>::epsilon() * 100) {
  return Catch::Matchers::Predicate<Quat>(
      [expected, epsilon](const Quat &actual)
      {
        return std::abs(actual.w - expected.w) < epsilon && std::abs(actual.x - expected.x) < epsilon &&
               std::abs(actual.y - expected.y) < epsilon && std::abs(actual.z - expected.z) < epsilon;
      },
      "Approximately equals " + Catch::StringMaker<Quat>::convert(expected)
  );
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
  REQUIRE_THAT(Quat::fromAxisAngle(axis, radians), Approximately({.w = -1, .x = 0, .y = 0, .z = 0}));
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
  REQUIRE_THAT(Quat::fromEuler(M_PI, 0, 0), Approximately({.w = 0, .x = 1, .y = 0, .z = 0}));
  // 180 degree rotation around Y
  REQUIRE_THAT(Quat::fromEuler(0, M_PI, 0), Approximately({.w = 0, .x = 0, .y = 1, .z = 0}));
  // 180 degree rotation around Z
  REQUIRE_THAT(Quat::fromEuler(0, 0, M_PI), Approximately({.w = 0, .x = 0, .y = 0, .z = 1}));
}

TEST_CASE("Quat normalized", "[quat]") {
  REQUIRE(Quat{2, 0, 0, 0}.normalized() == Quat::IDENTITY);
  REQUIRE_THAT(
      Quat(3, 5, 4, 7).normalized(),
      Approximately({.w = 0.30151134, .x = 0.50251891, .y = 0.40201513f, .z = 0.70352647f})
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
      a * b == Quat{
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
