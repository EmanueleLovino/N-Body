#pragma once
#include <cmath>

#include "constants.hpp"

namespace nbody::physics {

template <typename System>
void compute_accelerations(System& system) {
    using T = typename System::value_type;
    /// constants needeed
    constexpr auto G = constants::G;
    constexpr auto soft_squared = constants::soft * constants::soft;

    for (auto&& pi : system) {
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

            const auto r2 =
                rijx * rijx + rijy * rijy + rijz * rijz + soft_squared;

            const auto inv_r = 1.0f / std::sqrt(r2);
            const auto inv_r3 = inv_r * inv_r * inv_r;

            const auto ai = G * pj.m * inv_r3;

            sum_aix += ai * rijx;
            sum_aiy += ai * rijy;
            sum_aiz += ai * rijz;
        }

        pi.ax = sum_aix;
        pi.ay = sum_aiy;
        pi.az = sum_aiz;
    }
}
}  // namespace nbody::physics
