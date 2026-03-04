#pragma once
#include <cmath>
#include <random>

namespace nbody::utils {
/// @brief random initalization method for the whole system
/// @tparams system to initialize
/// @param number of particles of the whole simulation,
/// @paramrandom seed used in the mt engine for random number generation
template <typename System>
void init_galaxy(System& system, int nParticles, unsigned long seed = 24) {
    using T = System::value_type;

    /// creation of a uniform distribution with the mt engine
    std::mt19937 rng(seed);
    std::uniform_real_distribution<T> dist(0.0f, 1.0f);

    /// allocate space for nParticles in order to avoid multiple heap
    /// reallocations
    system.reserve(nParticles);

    /// add central massive body
    system.add_particle(
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0e24f, 0.0f});

    /// add the rest of the particles up to nParticles
    for (auto i = 1; i < nParticles; ++i) {
        T m = dist(rng) * 5e20f;
        T r = m * 2.5e-15f;

        T horizontalAngle = dist(rng) * 2.0f * M_PI;
        T verticalAngle = dist(rng) * 2.0f * M_PI;
        T distToCenter = dist(rng) * 1.0e8f + 1.0e8f;

        T qx =
            std::cos(verticalAngle) * std::sin(horizontalAngle) * distToCenter;
        T qy = std::sin(verticalAngle) * distToCenter;
        T qz =
            std::cos(verticalAngle) * std::cos(horizontalAngle) * distToCenter;

        T vx = qy * 4.0e-6;
        T vy = -qx * 4.0e-6;
        T vz = 0.0;

        system.add_particle({qx, qy, qz, vx, vy, vz, 0.0f, 0.0f, 0.0f, m, r});
    };
};
}  // namespace nbody::utils
