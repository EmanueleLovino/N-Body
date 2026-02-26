#include <cassert>
#include <iostream>
#include <vector>

#include "Bodies.hpp"
#include "Particles.hpp"

int main() {
    nbody::Bodies<std::vector, float, nbody::SoATag> universe;
    universe.addParticle(2, 3, 4, 5, 6, 7, 7, 8, 8, 10, 11);

    nbody::Bodies<std::vector, float, nbody::AoSTag> universe_2;
    universe_2.reserve(100);
    universe_2.addParticle(2, 3, 4, 5, 6, 7, 7, 8, 8, 10, 11);

    universe.addParticle(2, 3, 4, 5, 6, 7, 7, 8, 8, 10, 11);

    nbody::AoS_particles<std::vector, float> new_universe;

    auto particles = new_universe;

    assert(universe_2.size() == 1);

    for (auto p : particles) {
        p.ax = 1;
    }
    for (auto p : particles) {
        std::cout << p.ax;
    }

    assert(universe.size() == 2);
}
