//
// Created by user on 10/22/2026.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H
#define GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H

#include "Replacement.h"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace ga {

    template <class C>
    class GenerationalReplacement : public Replacement<C> {
    public:
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

            return std::vector<C>(
                    offspring.begin(),
                    offspring.begin() + populationSize
            );
        }
    };

}
#endif //GENETIC_ALGORITHM_TOOLBOX_GENERATIONALREPLACEMENT_H
