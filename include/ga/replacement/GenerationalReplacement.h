//
// Created by user on 10/22/2026.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H
#define GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H

#include "Replacement.h"

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace ga {

    template <class C>
    class GenerationalReplacement : public Replacement<C> {
    private:
        std::size_t eliteCount;
        bool minimize;

        bool isBetter(const C& first, const C& second) const {
            if (minimize) {
                return first.get_fitness() < second.get_fitness();
            }
            return first.get_fitness() > second.get_fitness();
        }

    public:
        explicit GenerationalReplacement(std::size_t eliteCount = 2, bool minimize = false)
                : eliteCount(eliteCount), minimize(minimize) {}

        std::vector<C> replace(
                const std::vector<C>& population,
                const std::vector<C>& offspring,
                std::size_t populationSize
        ) const override
        {

            if (populationSize == 0) {
                return {};
            }

            if (population.empty() || offspring.empty()) {
                throw std::invalid_argument(
                        "Population and offspring must not be empty"
                );
            }

            if (offspring.size() < populationSize) {
                throw std::invalid_argument(
                        "Offspring count must be at least the population size"
                );
            }

            // Same as before: the next generation starts as the offspring.
            std::vector<C> next(
                    offspring.begin(),
                    offspring.begin() + populationSize
            );

            std::size_t k = std::min(eliteCount, std::min(population.size(), next.size()));
            if (k == 0) {
                return next;
            }

            std::vector<std::size_t> parents(population.size());
            std::iota(parents.begin(), parents.end(), std::size_t(0));
            std::partial_sort(parents.begin(), parents.begin() + k, parents.end(),
                              [&](std::size_t a, std::size_t b) {
                                  return isBetter(population[a], population[b]);
                              });

            std::vector<std::size_t> slots(next.size());
            std::iota(slots.begin(), slots.end(), std::size_t(0));
            std::partial_sort(slots.begin(), slots.begin() + k, slots.end(),
                              [&](std::size_t a, std::size_t b) {
                                  return isBetter(next[b], next[a]);
                              });

            for (std::size_t i = 0; i < k; ++i) {
                if (isBetter(population[parents[i]], next[slots[i]])) {
                    next[slots[i]] = population[parents[i]];
                }
            }

            return next;
        }
    };

}
#endif //GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H