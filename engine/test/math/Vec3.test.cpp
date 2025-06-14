#include "math/algebras.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>

TEST_CASE("Vec3 length", "[vec3]") {
  REQUIRE(Vec3{0, 0, 0}.length() == 0);
  REQUIRE(Vec3{1, 0, 0}.length() == 1);
  REQUIRE(Vec3{0, 1, 0}.length() == 1);
  REQUIRE(Vec3{0, 0, 1}.length() == 1);
  REQUIRE(Vec3{1, 1, 1}.length() == Catch::Approx(1.7320508f));
  REQUIRE(Vec3{-1, 1, 1}.length() == Catch::Approx(1.7320508f));
  REQUIRE(Vec3{1, -1, 1}.length() == Catch::Approx(1.7320508f));
  REQUIRE(Vec3{1, 1, -1}.length() == Catch::Approx(1.7320508f));
}

TEST_CASE("Vec3 normalize", "[vec3]") {
  REQUIRE(Vec3{0, 0, 0}.normalize() == Vec3{0, 0, 0});
  REQUIRE(Vec3{1, 0, 0}.normalize() == Vec3{1, 0, 0});
  REQUIRE(Vec3{0, 1, 0}.normalize() == Vec3{0, 1, 0});
  REQUIRE(Vec3{0, 0, 1}.normalize() == Vec3{0, 0, 1});
  REQUIRE(Vec3{1, 2, 3}.normalize().length() == Catch::Approx(1));
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
    Quat identity{1, 0, 0, 0};
    REQUIRE(v.rotate(identity) == v);

    // 360 degree rotation around Z axis
    Vec3 axis{0, 0, 1};
    float radians = 2 * M_PI; // 360 degrees
    Quat rot360 = Quat::fromAxisAngle(axis, radians);
    Vec3 rotated = v.rotate(rot360);
    REQUIRE(rotated.x == Catch::Approx(v.x));
    REQUIRE(rotated.y == Catch::Approx(v.y));
    REQUIRE(rotated.z == Catch::Approx(v.z));

    // 180 degree rotation around X axis
    Vec3 axisX{1, 0, 0};
    Quat rot180 = Quat::fromAxisAngle(axisX, M_PI);
    Vec3 v2{1, 2, 3};
    Vec3 rotated180 = v2.rotate(rot180);
    REQUIRE(rotated180.x == Catch::Approx(1));
    REQUIRE(rotated180.y == Catch::Approx(-2));
    REQUIRE(rotated180.z == Catch::Approx(-3));
}
