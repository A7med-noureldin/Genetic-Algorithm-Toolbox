//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_FLOATCHROMOSOME_H
#define GENETIC_ALGORITHM_TOOLBOX_FLOATCHROMOSOME_H
#include "Chromosome.h"
#include "../core/Random.h"
#include <cstddef>

namespace ga
{
    struct FloatSpec
    {
        int length = 0;
        double lower = 0.0;
        double upper = 1.0;
    };
    class FloatChromosome: public Chromosome<double>
    {
    public:
        using Chromosome<double>::Chromosome;
        FloatChromosome() = default;

        FloatChromosome(const FloatSpec& spec, Random& rng)
        {
            gen.resize(spec.length);
            for (auto& g : gen)
                g = rng.next_real(spec.lower, spec.upper);
        }

        bool is_valid(const FloatSpec& spec) const
        {
            if (gen.size() != static_cast<std::size_t>(spec.length)) return false;
            for (auto g : gen)
                if (g < spec.lower || g > spec.upper) return false;
            return true;
        }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_FLOATCHROMOSOME_H