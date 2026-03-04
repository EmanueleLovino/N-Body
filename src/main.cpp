#include <functional>
#include <iostream>
#include <string>

#include "integrators/integrators.hpp"
#include "nbody.hpp"
#include "particles.hpp"
#include "utils/init_galaxy.hpp"

// default values
std::size_t NParticles = 1000;
unsigned long NIterations = 1000;
float Dt = 0.01f;
std::string IntegratorTag = "leapfrog";
bool Verbose = false;

void print_usage(const char* prog) {
    std::cout
        << "Usage: " << prog << " [options]\n"
        << "Options:\n"
        << "  -n  <nParticles>     number of particles (default: " << NParticles
        << ")\n"
        << "  -i  <nIter>       number of iterations (default: " << NIterations
        << ")\n"
        << "  -dt <timestep>    timestep (default: " << Dt << ")\n"
        << "  -im <integrator>  integrator: euler, verlet, leapfrog (default: "
        << IntegratorTag << ")\n"
        << "  -v                verbose mode\n"
        << "  -h                display this help\n";
}

void parse_args(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-n" && i + 1 < argc)
            NParticles = std::stoul(argv[++i]);
        else if (arg == "-i" && i + 1 < argc)
            NIterations = std::stoul(argv[++i]);
        else if (arg == "-dt" && i + 1 < argc)
            Dt = std::stof(argv[++i]);
        else if (arg == "-im" && i + 1 < argc)
            IntegratorTag = argv[++i];
        else if (arg == "-v")
            Verbose = true;
        else if (arg == "-h") {
            print_usage(argv[0]);
            exit(0);
        } else {
            std::cout << "Unknown argument: " << arg << "\n";
            print_usage(argv[0]);
            exit(-1);
        }
    }
}

int main(int argc, char** argv) {
    parse_args(argc, argv);

    // display configuration
    std::cout << "N-Body simulation configuration:\n"
              << "--------------------------------\n"
              << "  -> nb. of bodies     (-n ): " << NParticles << "\n"
              << "  -> nb. of iterations (-i ): " << NIterations << "\n"
              << "  -> timestep          (-dt): " << Dt << "\n"
              << "  -> integrator        (-im): " << IntegratorTag << "\n"
              << "  -> verbose mode      (-v ): "
              << (Verbose ? "enabled" : "disabled") << "\n\n";

    // create system
    using System = nbody::System<std::vector, float, SoA>;
    System system;
    nbody::utils::init_galaxy(system, NParticles, 42);

    // select integrator
    using Integrator = std::function<void(System&, float)>;
    Integrator integrator;
    if (IntegratorTag == "euler")
        integrator = [](auto& s, float dt) {
            nbody::integrators::euler(s, dt);
        };
    else if (IntegratorTag == "verlet")
        integrator = [](auto& s, float dt) {
            nbody::integrators::verlet(s, dt);
        };
    else if (IntegratorTag == "leapfrog")
        integrator = [](auto& s, float dt) {
            nbody::integrators::leapfrog(s, dt);
        };
    else {
        std::cout << "Unknown integrator: " << IntegratorTag << "\n";
        print_usage(argv[0]);
        return -1;
    }

    // create simulation
    nbody::Nbody sim(std::move(system), std::move(integrator), NIterations);

    double e_initial = sim.energy();
    std::cout << "Simulation started...\n";
    std::cout << "Initial energy: " << e_initial << "\n\n";

    // run simulation
    for (unsigned long i = 1; i <= NIterations; ++i) {
        sim.step(Dt);
        if (Verbose && i % 100 == 0) {
            std::cout << "Iteration " << i << "/" << NIterations
                      << "  energy: " << sim.energy() << "\r";
            std::cout.flush();
        }
    }

    double e_final = sim.energy();
    double drift = std::abs(e_final - e_initial) / std::abs(e_initial) * 100.0;

    std::cout << "\nSimulation ended.\n\n"
              << "Final energy:  " << e_final << "\n"
              << "Energy drift:  " << drift << "%\n";

    return 0;
}
