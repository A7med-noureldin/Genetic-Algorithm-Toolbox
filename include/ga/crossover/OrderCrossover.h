//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_ORDERCROSSOVER_H
#define GENETIC_ALGORITHM_TOOLBOX_ORDERCROSSOVER_H

#include "Crossover.h"
#include <random>
#include <algorithm>

namespace ga {
    template <typename Gene>
    class OrderCrossover : public Crossover<Gene> {
    public:
        using chromosome = typename Crossover<Gene>::chromosome;
        using offspring  = typename Crossover<Gene>::offspring;

        offspring crossover(const chromosome& parent1, const chromosome& parent2) const override;

    private:
        static chromosome buildChild(const chromosome& keep, const chromosome& other, std::size_t a, std::size_t b);
    };

    template <typename Gene>
    typename OrderCrossover<Gene>::chromosome OrderCrossover<Gene>::buildChild(const chromosome& keep, const chromosome& other, std::size_t a, std::size_t b) {
        const std::size_t n = keep.size();
        chromosome child = keep;

        std::size_t start = (b+1)%n;
        for (std::size_t i = 0; i < n; i++) {
            const Gene g = other[(b+1+i)%n];
            bool isIn = false;
            for (size_t j = a; j <= b; j++) {
                if (keep[j] == g) {
                    isIn = true;
                    break;
                }
            }
            if (isIn) continue;
            child[start] = g;
            start = (start+1)%n;
        }
        return child;
    }

    template <typename Gene>
    typename OrderCrossover<Gene>::offspring OrderCrossover<Gene>::crossover(const chromosome& parent1, const chromosome& parent2) const {
        const std::size_t n = parent1.size();
        if (n < 2) return {parent1, parent2};

        static thread_local std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<std::size_t> dist(0, n - 1);

        std::size_t a = dist(rng);
        std::size_t b = dist(rng);
        if (a > b) std::swap(a, b);

        return {buildChild(parent1, parent2, a, b), buildChild(parent2, parent1, a, b)};
    }
}

#endif //GENETIC_ALGORITHM_TOOLBOX_ORDERCROSSOVER_H