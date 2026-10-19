//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_INTEGERCHROMOSOME_H
#define GENETIC_ALGORITHM_TOOLBOX_INTEGERCHROMOSOME_H
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include "Chromosome.h"
#include "../core/Random.h"

namespace ga
{
    struct IntegerSpec
    {
        int length = 0;
        int lower = 0;
        int upper = 1;
        bool permutation = false;     // true: every value in [lower, upper] appears exactly once
    };

    class IntegerChromosome: public Chromosome<int>
    {
    public:
        using Chromosome<int>::Chromosome;
        IntegerChromosome() = default;

        IntegerChromosome(const IntegerSpec& spec, Random& rng)
        {
            gen.resize(spec.length);
            if (spec.permutation)
            {
                // a permutation only makes sense if the range has exactly 'length' values
                if (spec.upper - spec.lower + 1 != spec.length) throw std::invalid_argument("permutation needs upper - lower + 1 == length");
                std::iota(gen.begin(), gen.end(), spec.lower);   // lower, lower+1, ..., upper
                rng.shuffle(gen);                                // then shuffle with the shared RNG
            }
            else
            {
                for (auto& g : gen) g = rng.next_int(spec.lower, spec.upper);
            }
        }

        bool is_valid(const IntegerSpec& spec) const
        {
            if (gen.size() != static_cast<std::size_t>(spec.length)) return false;
            for (auto g : gen) if (g < spec.lower || g > spec.upper) return false;

            if (spec.permutation)
            {
                if (spec.upper - spec.lower + 1 != spec.length) return false;
                std::vector<bool> seen(spec.length, false);
                for (auto g : gen)
                {
                    if (seen[g - spec.lower]) return false;      // duplicate value
                    seen[g - spec.lower] = true;
                }
            }
            return true;
        }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_INTEGERCHROMOSOME_H