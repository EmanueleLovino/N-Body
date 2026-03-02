#include "particles.hpp"
#include <NBody.hpp>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
int main() {
    nbody::System<std::vector, float, AoS> new_universe;
    auto& particles = new_universe;
    nbody::Particle<float> p(2, 3, 4, 5, 6, 7, 7, 8, 8, 10, 11);
    particles.add_particle(p);

    nbody::Nbody<decltype(new_universe)> uni(std::move(new_universe));

    for (auto&& p : new_universe) {
        p.ax = 1;
    }
    for (auto&& p : new_universe) {
        std::cout << p.qx;
        std::cout << p.qy;
        std::cout << p.qz;
        std::cout << p.ax;
        std::cout << p.ay;
        std::cout << p.az;
        std::cout << p.r;
        std::cout << p.m;
    }
    for (auto&& p : uni.get()) {
        std::cout << p.qx;
        std::cout << p.qy;
        std::cout << p.qz;
        std::cout << p.ax;
        std::cout << p.ay;
        std::cout << p.az;
        std::cout << p.r;
        std::cout << p.m;
    }
}
