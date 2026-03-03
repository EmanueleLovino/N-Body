#pragma once
#include <cstddef>
#include <utility>

namespace nbody {

template <typename System, typename Integrator> class Nbody {

  public:
    using size_type = std::size_t;

    explicit Nbody(System&& system, Integrator&& integrator, size_type nIter)
        : system_(std::move(system)), integrator_(std::move(integrator)),
          nIter_(nIter) {}

    [[nodiscard]] size_type size() const { return system_.size(); }

    /// introduced for testing purposes
    System& get() { return system_; }

    void step() {}

  private:
    System system_;
    Integrator integrator_;
    /// softening factor, used to maintain a better numberical stability
    const float soft_{0.035f};
    /// number of iteration for the whole simulation
    size_type nIter_{0};
};

/// needed for CTAD: class template auto deduction, used in test_system (move
/// semantics)
template <typename System, typename Integrator>
Nbody(System&&, Integrator) -> Nbody<System, Integrator>;
} // namespace nbody
