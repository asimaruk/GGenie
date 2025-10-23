#include "math/algebras.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>

using namespace GGenie;

auto Approximately(const Vec3 &expected, float epsilon = std::numeric_limits<float>::epsilon() * 100) {
  return Catch::Matchers::Predicate<Vec3>(
      [expected, epsilon](const Vec3 &actual)
      {
        return std::abs(actual.x - expected.x) < epsilon && std::abs(actual.y - expected.y) < epsilon &&
               std::abs(actual.z - expected.z) < epsilon;
      },
      "Approximately equals " + Catch::StringMaker<Vec3>::convert(expected)
  );
}

TEST_CASE("Vec3 length", "[vec3]") {
  REQUIRE(Vec3{0, 0, 0}.length() == 0);
  REQUIRE(Vec3{1, 0, 0}.length() == 1);
  REQUIRE(Vec3{0, 1, 0}.length() == 1);
  REQUIRE(Vec3{0, 0, 1}.length() == 1);
  REQUIRE_THAT(Vec3(1, 1, 1).length(), Catch::Matchers::WithinRel(1.7320508f));
  REQUIRE_THAT(Vec3(-1, 1, 1).length(), Catch::Matchers::WithinRel(1.7320508f));
  REQUIRE_THAT(Vec3(1, -1, 1).length(), Catch::Matchers::WithinRel(1.7320508f));
  REQUIRE_THAT(Vec3(1, 1, -1).length(), Catch::Matchers::WithinRel(1.7320508f));
}

TEST_CASE("Vec3 normalize", "[vec3]") {
  REQUIRE(Vec3{0, 0, 0}.normalize() == Vec3{0, 0, 0});
  REQUIRE(Vec3{1, 0, 0}.normalize() == Vec3{1, 0, 0});
  REQUIRE(Vec3{0, 1, 0}.normalize() == Vec3{0, 1, 0});
  REQUIRE(Vec3{0, 0, 1}.normalize() == Vec3{0, 0, 1});
  REQUIRE_THAT(Vec3(1, 2, 3).normalize().length(), Catch::Matchers::WithinRel(1.f));
  REQUIRE_THAT(
      Vec3(1, 2, 3).normalize(),
      Catch::Matchers::Predicate<Vec3>(
          [](const Vec3 &vec)
          {
            return vec.x / 1 == vec.y / 2 && vec.y / 2 == vec.z / 3 && vec.x / 1 == vec.z / 3;
          },
          "Normalized vector should be collinear to original"
      )
  );
}

TEST_CASE("Vec3 addition and subtraction", "[vec3]") {
  Vec3 a{1, 2, 3};
  Vec3 b{4, 5, 6};
  REQUIRE((a + b) == Vec3{5, 7, 9});
  REQUIRE((b - a) == Vec3{3, 3, 3});
}

TEST_CASE("Vec3 scalar multiplication and division", "[vec3]") {
  Vec3 v{2, -4, 8};
  REQUIRE((v * 2) == Vec3{4, -8, 16});
  REQUIRE((v / 2) == Vec3{1, -2, 4});
}

TEST_CASE("Vec3 compound assignment operators", "[vec3]") {
  Vec3 v{1, 2, 3};
  v += Vec3{4, 5, 6};
  REQUIRE(v == Vec3{5, 7, 9});
  v -= Vec3{1, 2, 3};
  REQUIRE(v == Vec3{4, 5, 6});
  v *= 2;
  REQUIRE(v == Vec3{8, 10, 12});
  v /= 4;
  REQUIRE(v == Vec3{2, 2.5f, 3});
}

TEST_CASE("Vec3 equality and inequality", "[vec3]") {
  Vec3 a{1, 2, 3};
  Vec3 b{1, 2, 3};
  Vec3 c{3, 2, 1};
  REQUIRE(a == b);
  REQUIRE_FALSE(a == c);
  REQUIRE(a != c);
  REQUIRE_FALSE(a != b);
}

TEST_CASE("Vec3 static constants", "[vec3]") {
  REQUIRE(Vec3::ZERO == Vec3{0, 0, 0});
  REQUIRE(Vec3::ONE == Vec3{1, 1, 1});
}

TEST_CASE("Vec3 rotate with quaternion", "[vec3]") {
  Vec3 v{1, 2, 3};
  REQUIRE(v.rotate(Quat::IDENTITY) == v);

  // 360 degree rotation around Z axis
  Quat rot360 = Quat::fromAxisAngle({0, 0, 1}, 2 * M_PI);
  REQUIRE_THAT(v.rotate(rot360), Approximately(v));

  // 180 degree rotation around X axis
  Quat rot180 = Quat::fromAxisAngle({1, 0, 0}, M_PI);
  REQUIRE_THAT(Vec3(1, 2, 3).rotate(rot180), Approximately({.x = 1, .y = -2, .z = -3}));
}
