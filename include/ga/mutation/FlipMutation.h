#ifndef GENETIC_ALGORITHM_TOOLBOX_FLIPMUTATION_H
#define GENETIC_ALGORITHM_TOOLBOX_FLIPMUTATION_H

#include "Mutation.h"
#include "../chromosomes/BinaryChromosome.h"

namespace ga
{
    class FlipMutation : public Mutation<BinaryChromosome>
    {
    private:
        double gene_mutation_probability;

    public:
        explicit FlipMutation(double probability);

        void mutate(BinaryChromosome& chromosome, Random& rng,
                    int generation, int max_generations) const override;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_FLIPMUTATION_H
