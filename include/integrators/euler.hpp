#pragma once

namespace nbody::integrators {
/// @brief forward euler integrator, implemented as a composition of ::physics
/// free methods
/// @tparams a system of particles
template <typename System>
void euler(System& system, float dt) {
    physics::compute_accelerations(system);
};
}  // namespace nbody::integrators
