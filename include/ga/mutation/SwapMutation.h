#ifndef GENETIC_ALGORITHM_TOOLBOX_SWAPMUTATION_H
#define GENETIC_ALGORITHM_TOOLBOX_SWAPMUTATION_H

#include "Mutation.h"
#include "../chromosomes/IntegerChromosome.h"

namespace ga
{
    class SwapMutation : public Mutation<IntegerChromosome>
    {
    private:
        double gene_mutation_probability;

    public:
        explicit SwapMutation(double probability);

        void mutate(IntegerChromosome& chromosome, Random& rng,
                    int generation, int max_generations) const override;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_SWAPMUTATION_H
