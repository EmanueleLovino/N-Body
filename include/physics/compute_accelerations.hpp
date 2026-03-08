#pragma once
#include <sys/cdefs.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <execution>

#include "constants.hpp"

namespace nbody::physics {

constexpr __always_inline auto fast_rsqrt(float x) -> float;
/// @brief free method to compute the acceleration of each particle. The method
/// is parallelized with C++ std::for_each, taking parallel unseq as execution
/// policy (as the method is embarassingly parallel, there is no need to
/// synchronize threads
/// @tparams a system of particles
template <typename System>
void compute_accelerations(System& system) {
    using T = typename System::value_type;
    /// constants needeed
    constexpr auto G = constants::G;
    constexpr auto soft_squared = constants::soft * constants::soft;

    std::for_each(std::execution::par_unseq, system.begin(), system.end(),
                  [&](auto&& pi) {
                      const auto qxi = pi.qx;
                      const auto qyi = pi.qy;
                      const auto qzi = pi.qz;

                      auto sum_aix = T{};
                      auto sum_aiy = T{};
                      auto sum_aiz = T{};

                      for (auto&& pj : system) {
                          const auto rijx = pj.qx - qxi;
                          const auto rijy = pj.qy - qyi;
                          const auto rijz = pj.qz - qzi;

                          const auto r2 = rijx * rijx + rijy * rijy +
                                          rijz * rijz + soft_squared;

                          const auto inv_r = fast_rsqrt(r2);
                          const auto inv_r3 = inv_r * inv_r * inv_r;

                          const auto ai = G * pj.m * inv_r3;

                          sum_aix += ai * rijx;
                          sum_aiy += ai * rijy;
                          sum_aiz += ai * rijz;
                      }

                      pi.ax = sum_aix;
                      pi.ay = sum_aiy;
                      pi.az = sum_aiz;
                  });
}

constexpr __always_inline auto fast_rsqrt(float x) -> float {
    const float x2 = x * 0.5f;
    auto i = std::bit_cast<std::int32_t>(x);
    i = 0x5f3759df - (i >> 1);
    auto y = std::bit_cast<float>(i);
    y = y * (1.5f - (x2 * y * y));  // 1st Newton-Raphson iteration
    y = y * (1.5f -
             (x2 * y *
              y));  // 2nd iteration — more accurate, remove if speed > accuracy
    return y;
};
}  // namespace nbody::physics
