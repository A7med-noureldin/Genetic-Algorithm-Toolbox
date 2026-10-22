#ifndef GENETIC_ALGORITHM_TOOLBOX_NONUNIFORMMUTATION_H
#define GENETIC_ALGORITHM_TOOLBOX_NONUNIFORMMUTATION_H

#include "Mutation.h"
#include "../chromosomes/FloatChromosome.h"

namespace ga
{
    // Supports FloatChromosome; selected genes take progressively smaller steps.
    class NonUniformMutation : public Mutation<FloatChromosome>
    {
    private:
        FloatSpec spec;
        double gene_mutation_probability;
        double shape_parameter;

    public:
        NonUniformMutation(const FloatSpec& float_spec, double probability,
                           double shape = 2.0);

        void mutate(FloatChromosome& chromosome, Random& rng,
                    int generation, int max_generations) const override;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_NONUNIFORMMUTATION_H
