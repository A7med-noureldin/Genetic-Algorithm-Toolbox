//
// Created by user on 10/22/2026.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_TOURNAMENTSELECTION_H
#define GENETIC_ALGORITHM_TOOLBOX_TOURNAMENTSELECTION_H

#include "Selection.h"

#include <vector>
#include <cstddef>
#include <cmath>
#include <stdexcept>

namespace ga {

    template <class C>
    class TournamentSelection : public Selection<C> {
    private:
        std::size_t tournamentSize;

    public:
        explicit TournamentSelection(std::size_t size = 3)
            : tournamentSize(size)
        {
            if (tournamentSize == 0) {
                throw std::invalid_argument(
                    "Tournament size must be greater than zero"
                );
            }
        }

        std::vector<std::size_t> select(
            const std::vector<C>& population,
            std::size_t count,
            Random& randomGenerator
        ) const override {

            if (population.empty()) {
                if (count == 0)
                    return {};

                throw std::invalid_argument(
                    "Cannot select from empty population"
                );
            }

            if (count == 0)
                return {};

            std::vector<std::size_t> selectedIndices;
            selectedIndices.reserve(count);

            for (std::size_t i = 0; i < count; ++i) {
                std::size_t bestCandidateIndex =
                    static_cast<std::size_t>(randomGenerator.next_int( 0, static_cast<int>(population.size() - 1)));

                double bestFitness =
                    population[bestCandidateIndex].get_fitness();

                if (!std::isfinite(bestFitness)) {
                    throw std::invalid_argument(
                        "Fitness must be finite"
                    );
                }

                for (std::size_t j = 1; j < tournamentSize; ++j) {
                    std::size_t currentCandidateIndex =
                        static_cast<std::size_t>(randomGenerator.next_int(0, static_cast<int>(population.size() - 1)));

                    double currentCandidateFitness =
                        population[currentCandidateIndex].get_fitness();

                    if (!std::isfinite(currentCandidateFitness)) {
                        throw std::invalid_argument(
                            "Fitness must be finite"
                        );
                    }

                    if (currentCandidateFitness > bestFitness) {
                        bestCandidateIndex = currentCandidateIndex;
                        bestFitness = currentCandidateFitness;
                    }
                }

                selectedIndices.push_back(bestCandidateIndex);
            }

            return selectedIndices;
        }
    };
}


#endif //GENETIC_ALGORITHM_TOOLBOX_TOURNAMENTSELECTION_H
