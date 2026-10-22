
#include "UniformMutation.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

namespace ga
{
    UniformMutation::UniformMutation(const FloatSpec& float_spec, double probability)
        : spec(float_spec), gene_mutation_probability(probability)
    {
        if (spec.length < 0 ||
            !isfinite(spec.lower) ||
            !isfinite(spec.upper) ||
            spec.lower > spec.upper)
        {
            throw invalid_argument(
                "UniformMutation requires a valid FloatSpec");
        }

        if (!isfinite(probability) ||
            probability < 0.001 ||
            probability > 0.1)
        {
            throw invalid_argument(
                "UniformMutation probability must be between 0.001 and 0.1");
        }
    }

    void UniformMutation::mutate(FloatChromosome& chromosome, Random& rng,
                                 int, int) const
    {
        if (chromosome.size() != static_cast<size_t>(spec.length))
        {
            throw invalid_argument(
                "Float chromosome length does not match FloatSpec");
        }

        for (double gene : chromosome.genes())
        {
            if (!isfinite(gene) ||
                gene < spec.lower ||
                gene > spec.upper)
            {
                throw invalid_argument(
                    "UniformMutation requires genes within FloatSpec bounds");
            }
        }

        vector<pair<size_t, double>> replacements;

        for (size_t i = 0; i < chromosome.size(); ++i)
        {
            if (rng.chance(gene_mutation_probability))
            {
                double x = chromosome.genes()[i];
                double new_value;

                double r = rng.next_real(0.0, 1.0);

                if (r <= 0.5)
                {
                    double delta_L = x - spec.lower;
                    double change = rng.next_real(0.0, delta_L);

                    new_value = x - change;
                }
                else
                {
                    double delta_U = spec.upper - x;
                    double change = rng.next_real(0.0, delta_U);

                    new_value = x + change;
                }

                if (new_value != x)
                    replacements.emplace_back(i, new_value);
            }
        }

        if (replacements.empty())
            return;

        vector<double>& genes = chromosome.mutable_genes();

        for (const auto& [index, value] : replacements)
            genes[index] = value;
    }
}
