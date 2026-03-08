#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "integrators/integrators.hpp"
#include "nbody.hpp"
#include "particles.hpp"
#include "utils/init_galaxy.hpp"

// default values
std::size_t NParticles = 1000;
unsigned long NIterations = 1000;
float Dt = 0.01f;
std::string IntegratorTag = "leapfrog";
std::string LayoutTag = "SoA";
std::string ContainerTag = "vector";
bool Verbose = false;

void print_usage(const char* prog) {
    std::cout
        << "Usage: " << prog << " [options]\n"
        << "Options:\n"
        << "  -n  <nParticles>  number of particles (default: " << NParticles
        << ")\n"
        << "  -i  <nIter>       number of iterations (default: " << NIterations
        << ")\n"
        << "  -dt <timestep>    timestep (default: " << Dt << ")\n"
        << "  -im <integrator>  integrator: euler, verlet, leapfrog (default: "
        << IntegratorTag << ")\n"
        << "  -l  <layout>      layout: SoA, AoS (default: " << LayoutTag
        << ")\n"
        << "  -c  <container>   container: vector (default: " << ContainerTag
        << ")\n"
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
        else if (arg == "-l" && i + 1 < argc)
            LayoutTag = argv[++i];
        else if (arg == "-c" && i + 1 < argc)
            ContainerTag = argv[++i];
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

template <template <typename...> typename Container, typename Layout>
void run_simulation() {
    using System = nbody::System<Container, float, Layout>;
    using Integrator = std::function<void(System&, float)>;

    System system;
    nbody::utils::init_galaxy(system, NParticles, 42);

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
        exit(-1);
    }

    nbody::Nbody sim(std::move(system), std::move(integrator), NIterations);

    double e_initial = sim.energy();
    std::cout << "Simulation started...\n"
              << "Initial energy: " << e_initial << "\n\n";

    auto start_time = std::chrono::high_resolution_clock::now();

    for (unsigned long i = 1; i <= NIterations; ++i) {
        sim.step(Dt);
        if (Verbose && i % 100 == 0) {
            std::cout << "Iteration " << i << "/" << NIterations
                      << "  energy: " << sim.energy() << "\r";
            std::cout.flush();
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            end_time - start_time)
                            .count();

    auto fps = NIterations * 1000 / elapsed_time;

    double e_final = sim.energy();
    double drift = std::abs(e_final - e_initial) / std::abs(e_initial) * 100.0;
    std::cout << "\nSimulation ended.\n\n"
              << "Simulation time:  " << elapsed_time << " ms\n"
              << "Iterations/s:  " << fps << "\n"
              << "Final energy:  " << e_final << "\n"
              << "Energy drift:  " << drift << "%\n";
}

int main(int argc, char** argv) {
    parse_args(argc, argv);

    std::cout << "N-Body simulation configuration:\n"
              << "--------------------------------\n"
              << "  -> nb. of bodies     (-n ): " << NParticles << "\n"
              << "  -> nb. of iterations (-i ): " << NIterations << "\n"
              << "  -> timestep          (-dt): " << Dt << "\n"
              << "  -> integrator        (-im): " << IntegratorTag << "\n"
              << "  -> layout            (-l ): " << LayoutTag << "\n"
              << "  -> container         (-c ): " << ContainerTag << "\n"
              << "  -> verbose mode      (-v ): "
              << (Verbose ? "enabled" : "disabled") << "\n\n";

    if (LayoutTag == "SoA" && ContainerTag == "vector")
        run_simulation<std::vector, SoA>();
    else if (LayoutTag == "AoS" && ContainerTag == "vector")
        run_simulation<std::vector, AoS>();
    else {
        std::cout << "Unknown layout or container: " << LayoutTag << " / "
                  << ContainerTag << "\n";
        return -1;
    }

    return 0;
}
