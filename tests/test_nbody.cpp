#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "integrators/integrators.hpp"
#include "nbody.hpp"
#include "particles.hpp"
#include "utils/init_galaxy.hpp"

using AoS_system = nbody::System<std::vector, float, AoS>;
using SoA_system = nbody::System<std::vector, float, SoA>;

/// ==================== nbody tests ====================
TEMPLATE_TEST_CASE("energy of the system should be conserved", "[integration]",
                   SoA_system, AoS_system) {
    TestType s;

    // initalize a galaxy with 100 particles, seed 42
    nbody::utils::init_galaxy(s, 100, 42);

    /// ==================== euler tests ====================
    SECTION("euler") {
        nbody::Nbody sim(
            std::move(s),
            [](auto& system, float dt) {
                nbody::integrators::euler(system, dt);
            },
            1000);

        // compute initial energy
        double e_initial = sim.energy();

        // run simulation
        for (int i = 0; i < 10000; ++i) {
            sim.step(0.01f);
        }

        double e_final = sim.energy();

        // energy should be conserved within tolerance
        REQUIRE(e_final == Catch::Approx(e_initial).epsilon(0.01));
    }

    /// ==================== verlet tests ====================
    SECTION("verlet") {
        nbody::Nbody sim(
            std::move(s),
            [](auto& system, float dt) {
                nbody::integrators::verlet(system, dt);
            },
            1000);

        // compute initial energy
        double e_initial = sim.energy();

        // run simulation
        for (int i = 0; i < 10000; ++i) {
            sim.step(0.01f);
        }

        double e_final = sim.energy();

        // energy should be conserved within tolerance
        REQUIRE(e_final == Catch::Approx(e_initial).epsilon(0.01));
    }

    /// ==================== leapfrog tests ====================
    SECTION("leapfrog") {
        nbody::Nbody sim(
            std::move(s),
            [](auto& system, float dt) {
                nbody::integrators::leapfrog(system, dt);
            },
            1000);

        // compute initial energy
        double e_initial = sim.energy();

        // run simulation
        for (int i = 0; i < 10000; ++i) {
            sim.step(0.01f);
        }

        double e_final = sim.energy();

        // energy should be conserved within tolerance
        REQUIRE(e_final == Catch::Approx(e_initial).epsilon(0.01));
    }
}
