//
// Created by matias on 22/06/25.
//

#include "gun.h"

#include <random>

double Gun::generate_random_number(double a, double b) {
    // thread_local: las partidas corren en hilos distintos y un motor compartido es un data race
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng);
}
