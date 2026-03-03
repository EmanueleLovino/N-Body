#include "nbody.hpp"
#include "particles.hpp"
#include "utils/init_galaxy.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <vector>
/// useful aliases for better clarity during testing, tests can be later
/// extended to other vector-like containers
using AoS_system = nbody::System<std::vector, float, AoS>;
using SoA_system = nbody::System<std::vector, float, SoA>;

TEMPLATE_TEST_CASE("init_galaxy initializes particles correctly", "[init]",
                   AoS_system, SoA_system) {

    TestType s;
    nbody::init_galaxy(s, 100, 42); // 100 particles, fixed seed

    SECTION("correct number of particles") { REQUIRE(s.size() == 100); }

    SECTION("central body is correct") {
        auto p = *s.begin();
        REQUIRE(p.m == Catch::Approx(2.0e24f));
        REQUIRE(p.qx == 0.0f);
        REQUIRE(p.qy == 0.0f);
        REQUIRE(p.qz == 0.0f);
        REQUIRE(p.vx == 0.0f);
        REQUIRE(p.vy == 0.0f);
        REQUIRE(p.vz == 0.0f);
    }

    SECTION("orbiting particles have non-zero positions") {
        int i = 0;
        for (auto&& p : s) {
            if (i != 0) { // skip central body
                REQUIRE(p.m > 0.0f);
                REQUIRE(p.r > 0.0f);
            }
            i++;
        }
    }

    SECTION("accelerations start at zero") {
        for (auto&& p : s) {
            REQUIRE(p.ax == 0.0f);
            REQUIRE(p.ay == 0.0f);
            REQUIRE(p.az == 0.0f);
        }
    }

    SECTION("reproducible with same seed") {
        TestType s2;
        nbody::init_galaxy(s2, 100, 42); // same seed
        auto p1 = *std::next(s.begin(), 1);
        auto p2 = *std::next(s2.begin());
        REQUIRE(p1.qx == Catch::Approx(p2.qx));
        REQUIRE(p1.qy == Catch::Approx(p2.qy));
    }

    SECTION("different seeds give different results") {
        TestType s2;
        nbody::init_galaxy(s2, 100, 999); // different seed
        auto p1 = *std::next(s.begin(), 1);
        auto p2 = *std::next(s2.begin());
        REQUIRE(p1.qx != Catch::Approx(p2.qx));
    }
}
