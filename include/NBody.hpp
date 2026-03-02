#pragma once
#include <cstddef>
#include <utility>

namespace nbody {

template <typename System, typename Integrator> class Nbody {

  public:
    using size_type = std::size_t;

    explicit Nbody(System&& system, Integrator&& integrator)
        : system_(std::move(system)), integrator_(integrator) {}

    [[nodiscard]] size_type size() const { return system_.size(); }

    /// introduced for testing purposes
    System& get() { return system_; }

  private:
    System system_;
    Integrator integrator_;
};

/// needed for CTAD: class template auto deduction, used in test_system (move
/// semantics)
template <typename System, typename Integrator>
Nbody(System&&, Integrator) -> Nbody<System, Integrator>;
} // namespace nbody
