#include "FlipMutation.h"

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <vector>

using namespace std;

namespace ga
{
    FlipMutation::FlipMutation(double probability)
        : gene_mutation_probability(probability)
    {
        if (!isfinite(probability) || probability < 0.001 || probability > 0.1)
            throw invalid_argument(
                "FlipMutation probability must be between 0.001 and 0.1");
    }

    void FlipMutation::mutate(BinaryChromosome &chromosome, Random &rng, int, int) const
    {
        for (uint8_t gene : chromosome.genes())
            if (gene > 1)
                throw invalid_argument("FlipMutation requires binary genes");

        vector<size_t> indices_to_flip;
        for (size_t i = 0; i < chromosome.size(); ++i)
            if (rng.chance(gene_mutation_probability))
                indices_to_flip.push_back(i);

        if (indices_to_flip.empty())
            return;

        vector<uint8_t> &genes = chromosome.mutable_genes();
        for (size_t index : indices_to_flip)
            genes[index] = static_cast<uint8_t>(1 - genes[index]);
    }
}
