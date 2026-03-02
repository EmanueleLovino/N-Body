#pragma once
#include <cstddef>
#include <utility>

namespace nbody {

template <typename System> class Nbody {
    using size_type = std::size_t;

  public:
    explicit Nbody(System&& system) : system_(std::move(system)) {};

    [[nodiscard]] size_type size() { return system_.size(); }

    System& get() { return system_; }

  private:
    System system_;
};

} // namespace nbody
