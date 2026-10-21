//
// Created by hazem on 10/19/26.
// Edited by Salma on 22/10/26.
//
// Member 4 Works here

#ifndef GENETIC_ALGORITHM_TOOLBOX_MUTATION_H
#define GENETIC_ALGORITHM_TOOLBOX_MUTATION_H

#include "../core/Random.h"

namespace ga
{
    template <class C>
    class Mutation
    {
    public:
        virtual ~Mutation() = default;

        virtual void mutate(C& chromosome, Random& rng,
                            int generation, int max_generations) const = 0;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_MUTATION_H