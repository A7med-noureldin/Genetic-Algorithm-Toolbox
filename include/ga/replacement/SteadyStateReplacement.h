//
// Created by user on 10/22/2026.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_STEADYSTATEREPLACEMENT_H
#define GENETIC_ALGORITHM_TOOLBOX_STEADYSTATEREPLACEMENT_H

#include "Replacement.h"
#include "../core/GAConfig.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace ga {

    template <class C>
    class SteadyStateReplacement : public Replacement<C> {
    private:
        bool minimize;
        std::size_t replacementCount;

        bool isBetter(const C& first, const C& second) const {
            if (minimize) {
                return first.get_fitness() < second.get_fitness();
            }
            return first.get_fitness() > second.get_fitness();
        }

    public:
        explicit SteadyStateReplacement( const GAConfig& config, std::size_t replacementCount = 2)
                : minimize(config.minimize), replacementCount(replacementCount) {}

        std::vector<C> replace(
                const std::vector<C>& population,
                const std::vector<C>& offspring,
                std::size_t populationSize
        ) const override
        {
            if (populationSize == 0) return {};

            if (population.size() != populationSize)
                throw std::invalid_argument("Population size does not match populationSize");


            std::vector<C> nextPopulation = population;
            std::vector<C> sortedOffspring = offspring;

            for (std::size_t i = 0; i < sortedOffspring.size(); ++i) {
                for (std::size_t j = i + 1; j < sortedOffspring.size(); ++j) {
                    if (isBetter(sortedOffspring[j], sortedOffspring[i])) {
                        std::swap(sortedOffspring[i], sortedOffspring[j]);
                    }
                }
            }

            std::vector<std::size_t> indices(populationSize);

            for (std::size_t i = 0; i < populationSize; ++i) {
                indices[i] = i;
            }

            for (std::size_t i = 0; i < indices.size(); ++i) {
                for (std::size_t j = i + 1; j < indices.size(); ++j) {
                    if (isBetter(nextPopulation[indices[i]],nextPopulation[indices[j]])) {
                        std::swap(indices[i], indices[j]);
                    }
                }
            }

            std::size_t count = std::min(populationSize, std::min(replacementCount, sortedOffspring.size()));

            for (std::size_t i = 0; i < count; ++i) {
                nextPopulation[indices[i]] = sortedOffspring[i];
            }
            return nextPopulation;
        }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_STEADYSTATEREPLACEMENT_H
