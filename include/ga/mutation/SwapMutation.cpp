
#include "SwapMutation.h"

#include <cmath>
#include <utility>

using namespace std;

namespace ga
{
    SwapMutation::SwapMutation(double probability)
        : gene_mutation_probability(probability)
    {
        if (!isfinite(probability) ||
            probability < 0.001 ||
            probability > 0.1)
        {
            throw invalid_argument(
                "SwapMutation probability must be between 0.001 and 0.1");
        }
    }

    void SwapMutation::mutate(IntegerChromosome &chromosome,
                              Random &rng, int, int) const
    {
        int length = static_cast<int>(chromosome.size());

        if (length < 2)
            return;

        vector<int> &genes = chromosome.mutable_genes();

        for (int i = 0; i < length; i++)
        {
            if (rng.chance(gene_mutation_probability))
            {
                int j = rng.next_int(0, length - 1);

                while (j == i)
                    j = rng.next_int(0, length - 1);

                swap(genes[i], genes[j]);
            }
        }
    }
}
