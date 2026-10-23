//
// Created by hazem on 10/19/26.
//
#ifndef GENETIC_ALGORITHM_TOOLBOX_MULTIPOINTCROSSOVER_H
#define GENETIC_ALGORITHM_TOOLBOX_MULTIPOINTCROSSOVER_H

#include "Crossover.h"
#include "../chromosomes/Chromosome.h"
#include <algorithm>
#include <numeric>
#include <random>

namespace ga{
    template <typename Gene>
    class MultiPointCrossover : public Crossover<Gene> {
    private:
        int numberOfPoints;
    public:
        using chromosome = typename Crossover<Gene>::chromosome;
        using offspring = typename Crossover<Gene>::offspring;
        explicit MultiPointCrossover(int x) : numberOfPoints(x){}
        offspring crossover(const chromosome& parent1, const chromosome& parent2) const override;
    };

    template <typename Gene>
    typename MultiPointCrossover<Gene>::offspring MultiPointCrossover<Gene>::crossover(const chromosome& parent1, const chromosome& parent2) const {
        const std::size_t n = parent1.size();
        chromosome offspring1 = parent1, offspring2 = parent2;

        if (numberOfPoints <= 0 || n < 2) return {offspring1, offspring2};

        const std::size_t k = std::min<std::size_t>(numberOfPoints, n - 1);
        std::vector<std::size_t> positions(n - 1);
        std::iota(positions.begin(), positions.end(), 1);

        static thread_local std::mt19937 rng{std::random_device{}()};
        std::shuffle(positions.begin(), positions.end(), rng);
        positions.resize(k);
        std::sort(positions.begin(), positions.end());
        positions.push_back(n);

        bool swapSegment = false;
        std::size_t prev = 0;
        for (std::size_t cut : positions) {
            if (swapSegment) {
                for (std::size_t i = prev; i < cut; i++)
                    std::swap(offspring1[i], offspring2[i]);
            }
            swapSegment = !swapSegment, prev = cut;
        }

        return {offspring1, offspring2};
    }
}


#endif //GENETIC_ALGORITHM_TOOLBOX_MULTIPOINTCROSSOVER_H