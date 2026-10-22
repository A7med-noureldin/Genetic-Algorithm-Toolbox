//
// Created by user on 10/21/2026.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_ROULETTEWHEELSELECTION_H
#define GENETIC_ALGORITHM_TOOLBOX_ROULETTEWHEELSELECTION_H

#include "Selection.h"

#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace ga{

    template <class C>
    class RouletteWheelSelection : public Selection<C>{
    public:
        std::vector <std::size_t> select(
                const std::vector<C>& population,
                std::size_t count,
                Random& randomGenerator
                ) const override
        {

            if(population.empty()){
                if(count == 0) return {};

                throw std::invalid_argument("Cannot select from empty population");
            }

            if(count == 0) return {};

            std::vector<double> cumulativeFitness;
            cumulativeFitness.reserve(population.size());

            double totalFitness = 0.0;

            for(const auto& chromosome : population){
                double fitness = chromosome.get_fitness();

                if(!std::isfinite(fitness) || fitness < 0){
                    throw std::invalid_argument("Roulette Wheel requires non negative fitness");
                }

                totalFitness+= fitness;
                if (!std::isfinite(totalFitness)) {
                    throw std::invalid_argument(
                            "Total fitness is too large."
                    );
                }
                cumulativeFitness.push_back(totalFitness);
            }

            std::vector<std::size_t> selected;
            selected.reserve(count);

            if (totalFitness == 0.0) {
                for (std::size_t i = 0; i < count; ++i) {
                    selected.push_back(static_cast<std::size_t>(
                                               randomGenerator.next_int(
                                                       0,
                                                       static_cast<int>(population.size() - 1)
                                               )
                                       ));
                }

                return selected;
            }

            for (std::size_t i = 0; i < count; ++i) {
                double randomTarget  = randomGenerator.next_real(0.0, totalFitness);

                auto it = std::upper_bound(cumulativeFitness.begin(), cumulativeFitness.end(), RandomTarget);
                if(it == cumulativeFitness.end()){
                    selected.push_back(static_cast<std::size_t>(population.size() - 1));
                }else {
                    selected.push_back(static_cast<std::size_t>(it - cumulativeFitness.begin()));
                }
            }
            return selected;
        }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_ROULETTEWHEELSELECTION_H
