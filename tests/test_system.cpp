#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "nbody.hpp"
#include "particles.hpp"
/// useful aliases for better clarity during testing, tests can be later
/// extended to other vector-like containers
using AoS_system = nbody::System<std::vector, float, AoS>;
using SoA_system = nbody::System<std::vector, float, SoA>;

TEMPLATE_TEST_CASE("move semantics in Nbody class works as intended", "[Nbody]",
                   AoS_system, SoA_system) {
    using SystemType = TestType;

    SystemType s;

    nbody::Particle<float> p(2, 3, 4, 5, 6, 7, 7, 8, 8, 10, 11);
    s.add_particle(p);
    s.add_particle(p);

    nbody::Nbody universe(std::move(s), [](auto& h, float dt) {}, 100);

    REQUIRE(s.size() == 0);
    /// moved on type as the same data as the moved from type
    REQUIRE(universe.size() == 2);
}
