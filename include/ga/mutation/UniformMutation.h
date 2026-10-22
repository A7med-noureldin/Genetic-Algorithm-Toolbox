#ifndef GENETIC_ALGORITHM_TOOLBOX_UNIFORMMUTATION_H
#define GENETIC_ALGORITHM_TOOLBOX_UNIFORMMUTATION_H

#include "Mutation.h"
#include "../chromosomes/FloatChromosome.h"

namespace ga
{
    // Supports FloatChromosome; selected genes are replaced within FloatSpec bounds.
    class UniformMutation : public Mutation<FloatChromosome>
    {
    private:
        FloatSpec spec;
        double gene_mutation_probability;

    public:
        UniformMutation(const FloatSpec& float_spec, double probability);

        void mutate(FloatChromosome& chromosome, Random& rng,
                    int generation, int max_generations) const override;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_UNIFORMMUTATION_H
