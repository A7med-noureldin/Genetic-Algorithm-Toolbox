//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_BINARYCHROMOSOME_H
#define GENETIC_ALGORITHM_TOOLBOX_BINARYCHROMOSOME_H
#include <cstdint>
#include <cstddef>
#include "Chromosome.h"
#include "../core/Random.h"

namespace ga
{
    struct BinarySpec
    {
        int length = 0;
    };

    class BinaryChromosome : public Chromosome<uint8_t>
    {
    public:
        using Chromosome<uint8_t>::Chromosome;
        BinaryChromosome() = default;

        BinaryChromosome(const BinarySpec& spec, Random& rng)
        {
            gen.resize(spec.length);
            for (auto& g : gen) g = static_cast<uint8_t>(rng.next_int(0, 1));   // generated as int, then converted
        }

        bool is_valid(const BinarySpec& spec) const
        {
            if (gen.size() != static_cast<std::size_t>(spec.length)) return false;
            for (auto g : gen) if (g > 1) return false;
            return true;
        }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_BINARYCHROMOSOME_H