#pragma once
#include <cstddef>
#include <utility>

#include "utils/compute_energy.hpp"

namespace nbody {

template <typename System, typename Integrator>
class Nbody {
   public:
    using size_type = std::size_t;

    explicit Nbody(System&& system, Integrator&& integrator, size_type nIter)
        : system_(std::move(system)),
          integrator_(std::move(integrator)),
          nIter_(nIter) {}

    [[nodiscard]] size_type size() const { return system_.size(); }

    void step(float dt) { integrator_(system_, dt); }

    /// @brief computes total energy of the system
    [[nodiscard]] auto energy() {
        return nbody::utils::compute_energy(system_);
    }

   private:
    System system_;
    Integrator integrator_;
    /// softening factor, used to maintain a better numberical stability
    /// number of iteration for the whole simulation
    size_type nIter_{0};
};

/// needed for CTAD: class template auto deduction, used in test_system (move
/// semantics)
template <typename System, typename Integrator>
Nbody(System&&, Integrator, std::size_t) -> Nbody<System, Integrator>;
}  // namespace nbody
