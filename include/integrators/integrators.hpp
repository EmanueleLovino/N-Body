#pragma once
#include "physics/compute_accelerations.hpp"
#include "physics/updates.hpp"

/// integrators are implemented as free functions which are themselves a simple
/// composition of different methods, this permitted to reduce boiler-plate
/// while providing as well lot of extensibility

namespace nbody::integrators {

/// @brief forward euler
/// @param system the particle system
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void euler(System& system, float dt) {
    physics::compute_accelerations(system);
    physics::update_velocities(system, dt);
    physics::update_positions(system, dt);
}

/// @brief verlet integrator
/// @param system the particle system
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void verlet(System& system, float dt) {
    physics::compute_accelerations(system);
    physics::update_positions_and_velocities(system, dt);
}

/// @brief leapfrog integrator
/// @param system the particle system
/// @param dt timestep
template <typename System>
    requires particles_system<System>
void leapfrog(System& system, float dt) {
    physics::update_velocities(system, dt * 0.5f);
    physics::update_positions(system, dt);
    physics::compute_accelerations(system);
    physics::update_velocities(system, dt * 0.5f);
}
}  // namespace nbody::integrators
