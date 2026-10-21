//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_CROSSOVER_H
#define GENETIC_ALGORITHM_TOOLBOX_CROSSOVER_H
#include <vector>
#include <utility>
#include "../chromosomes/Chromosome.h"

namespace ga{
    template <typename Gene>
    class Crossover {
    public:
        using chromosome = Chromosome<Gene>;
        using offspring = std::pair<chromosome, chromosome>;
        virtual ~Crossover() = default;
        virtual offspring crossover(const chromosome& parent1, const chromosome& parent2) const = 0;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_CROSSOVER_H