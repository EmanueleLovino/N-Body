#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <vector>

#include "constants.hpp"
#include "particles.hpp"
#include "physics/compute_accelerations.hpp"

/// useful aliases for better clarity during testing, tests can be later
/// extended to other vector-like containers
using AoS_system = nbody::System<std::vector, float, AoS>;
using SoA_system = nbody::System<std::vector, float, SoA>;

/// tests for the utils directory free methods

TEMPLATE_TEST_CASE("compute_accelerations", "[physics]", SoA_system,
                   AoS_system) {
    SECTION("two particles accelerate towards each other") {
        TestType s;
        // particle 0 at origin, particle 1 at (1, 0, 0)
        s.add_particle({0, 0, 0, 0, 0, 0, 0, 0, 0, 1.0f, 0.1f});
        s.add_particle({1, 0, 0, 0, 0, 0, 0, 0, 0, 1.0f, 0.1f});

        nbody::physics::compute_accelerations(s);

        // a = G * m / r^2, directed towards other particle
        // for particle 0: ax should be positive (towards x=1)
        // for particle 1: ax should be negative (towards x=0)
        auto p0 = *s.begin();
        auto p1 = *std::next(s.begin(), 1);

        REQUIRE(p0.ax > 0);  // attracted towards particle 1
        REQUIRE(p1.ax < 0);  // attracted towards particle 0
        REQUIRE(p0.ax == Catch::Approx(-p1.ax));  // equal and opposite
        REQUIRE(p0.ay == Catch::Approx(0.0f));
        REQUIRE(p0.az == Catch::Approx(0.0f));
    }

    SECTION("single particle has zero acceleration") {
        TestType s;
        s.add_particle({0, 0, 0, 0, 0, 0, 0, 0, 0, 1.0f, 0.1f});
        nbody::physics::compute_accelerations(s);
        auto p = *s.begin();
        REQUIRE(p.ax == Catch::Approx(0.0f));
        REQUIRE(p.ay == Catch::Approx(0.0f));
        REQUIRE(p.az == Catch::Approx(0.0f));
    }

    SECTION("acceleration magnitude matches formula") {
        TestType s;
        s.add_particle({0, 0, 0, 0, 0, 0, 0, 0, 0, 1.0f, 0.1f});
        s.add_particle({1, 0, 0, 0, 0, 0, 0, 0, 0, 2.0f, 0.1f});

        nbody::physics::compute_accelerations(s);

        // a = G * m2 / (r^2 + soft^2)^(3/2)
        constexpr float G = nbody::constants::G;
        constexpr float soft = nbody::constants::soft;
        float r2 = 1.0f + soft * soft;
        float expected = G * 2.0f / (r2 * std::sqrt(r2));

        auto p0 = *s.begin();
        REQUIRE(p0.ax == Catch::Approx(expected).epsilon(1e-5));
    }
}
