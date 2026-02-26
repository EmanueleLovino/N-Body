#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <vector>

#include "Bodies.hpp"

using SoABodies = nbody::Bodies<std::vector, float, nbody::SoATag>;

using AoSBodies = nbody::Bodies<std::vector, float, nbody::SoATag>;

// verifying that the compile-time dispatch through Tags work as intended

// SoATag to SoAData
static_assert(
    std::same_as<nbody::StorageTraits<nbody::SoATag, std::vector, float>::type,
                 nbody::detail::SoAData<std::vector, float>>);

// AoSTag to AoSData
static_assert(
    std::same_as<nbody::StorageTraits<nbody::AoSTag, std::vector, float>::type,
                 nbody::detail::AoSData<std::vector, float>>);

/*
 * Using the Catch2 unit test framework, in particular the TEMPLATE_TEST_CASE
 * feature permits us to extend the testing to different memory layouts without
 * adding eccessive boilerplate
 */

// construction
TEMPLATE_TEST_CASE("default constructed object should have size 0", "[Bodies]",
                   SoABodies, AoSBodies) {
    TestType b;
    REQUIRE(b.size() == 0u);
}

// adding a particle should result in size++
TEMPLATE_TEST_CASE("adding elements correctly", "[Bodies]", SoABodies,
                   AoSBodies) {
    TestType b;
    b.addParticle(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    REQUIRE(b.size() == 1u);
}

// reserve should not change the size of the underlying data
TEMPLATE_TEST_CASE(" reserve should not change the size of the underlying data",
                   "[Bodies]", SoABodies, AoSBodies) {
    TestType b;
    b.addParticle(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    b.reserve(100);
    REQUIRE(b.size() == 1u);
}

TEST_CASE("iterate over all particles", "[bodies]") {
    nbody::Bodies<std::vector, float, nbody::SoATag> b;

    // add some particles
    for (int i = 1; i <= 5; ++i) {
        auto v = static_cast<float>(i);
        b.addParticle(v, v, v,  // qx qy qz
                      v, v, v,  // vx vy vz
                      0, 0, 0,  // ax ay az
                      v, v);    // m  r
    }

    // iterate and modify
    for (std::size_t i = 0; i < b.size(); ++i) {
        b.qx(i) *= 2.f;
    }

    // verify
    for (std::size_t i = 0; i < b.size(); ++i) {
        float expected = static_cast<float>(i + 1) * 2.f;
        CHECK(b.qx(i) == Catch::Approx(expected));
    }
}
