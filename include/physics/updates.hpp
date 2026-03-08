#pragma once
#include "concepts.hpp"
namespace nbody::physics {

/// @brief updates velocities from accelerations: v += a * dt
/// @tparam System a particle system
/// @param system the particle system to update
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void update_velocities(System& system, float dt) {
    using T = typename System::value_type;

    const T dt_ = dt;

    for (auto&& p : system) {
        p.vx += p.ax * dt_;
        p.vy += p.ay * dt_;
        p.vz += p.az * dt_;
    }
}

/// @brief updates positions from velocities: q += v * dt
/// @tparam System a particle system
/// @param system the particle system to update
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void update_positions(System& system, float dt) {
    using T = typename System::value_type;

    const T dt_ = dt;

    for (auto&& p : system) {
        p.qx += p.vx * dt_;
        p.qy += p.vy * dt_;
        p.qz += p.vz * dt_;
    }
}

/// @brief updates positions and velocities using the Verlet scheme
/// @tparam System a particle system (AoS or SoA)
/// @param system the particle system to update
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void update_positions_and_velocities(System& system, float dt) {
    using T = typename System::value_type;

    const T dt_ = dt;

    for (auto&& p : system) {
        T ax_dt = p.ax * dt_;
        T ay_dt = p.ay * dt_;
        T az_dt = p.az * dt_;
        p.qx += (p.vx + ax_dt * T{0.5}) * dt_;
        p.qy += (p.vy + ay_dt * T{0.5}) * dt_;
        p.qz += (p.vz + az_dt * T{0.5}) * dt_;
        p.vx += ax_dt;
        p.vy += ay_dt;
        p.vz += az_dt;
    }
}

}  // namespace nbody::physics
