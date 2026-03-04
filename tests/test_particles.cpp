#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "particles.hpp"

/// useful aliases for better clarity during testing, tests can be later
/// extended to other vector-like containers
using AoS_system = nbody::System<std::vector, float, AoS>;
using SoA_system = nbody::System<std::vector, float, SoA>;

/// Using the catch2 unit test framework permits us to use the
/// TEMPLATE_TEST_CASE, enabling the testing of multiple memory-layouts without
/// adding eccessive boiler-plate

TEMPLATE_TEST_CASE("default size of the system is 0", "[System]", AoS_system,
                   SoA_system) {
    TestType s;
    REQUIRE(s.size() == 0u);
}

TEMPLATE_TEST_CASE("reserve() method doesn't change size", "[System]",
                   AoS_system, SoA_system) {
    TestType s;
    s.reserve(100);
    REQUIRE(s.size() == 0u);
}

TEMPLATE_TEST_CASE("adding a Particle results in a size + 1", "[System]",
                   AoS_system, SoA_system) {
    TestType s;
    s.add_particle(nbody::Particle<float>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
    REQUIRE(s.size() == 1u);
}

TEMPLATE_TEST_CASE("Range-based for loops work as intended", "[System]",
                   AoS_system, SoA_system) {
    using SystemType = TestType;
    SystemType s;

    s.reserve(3);
    s.add_particle(nbody::Particle<float>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
    s.add_particle(nbody::Particle<float>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
    s.add_particle(nbody::Particle<float>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));

    /// modyfing a field
    for (auto&& p : s) {
        p.qx = 2;
    }
    float sum = 0;

    /// summing over a field
    for (auto&& p : s) {
        sum += p.qx;
    }
    REQUIRE(sum == 6);
}
