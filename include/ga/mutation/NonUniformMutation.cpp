
#include "NonUniformMutation.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

namespace ga
{
    NonUniformMutation::NonUniformMutation(
        const FloatSpec& float_spec,
        double probability,
        double b_value)
        : spec(float_spec),
          gene_mutation_probability(probability),
          shape_parameter(b_value)
    {
        if (spec.length < 0 ||
            !isfinite(spec.lower) ||
            !isfinite(spec.upper) ||
            spec.lower > spec.upper)
        {
            throw invalid_argument(
                "NonUniformMutation requires a valid FloatSpec");
        }

        if (!isfinite(probability) ||
            probability < 0.001 ||
            probability > 0.1)
        {
            throw invalid_argument(
                "Mutation probability must be between 0.001 and 0.1");
        }

        if (!isfinite(shape_parameter) || shape_parameter <= 0)
        {
            throw invalid_argument(
                "b must be greater than zero");
        }
    }

    void NonUniformMutation::mutate(
        FloatChromosome& chromosome,
        Random& rng,
        int generation,
        int max_generations) const
    {
        if (chromosome.size() != static_cast<size_t>(spec.length))
        {
            throw invalid_argument(
                "Float chromosome length does not match FloatSpec");
        }

        if (max_generations <= 0 ||
            generation < 0 ||
            generation >= max_generations)
        {
            throw invalid_argument("Invalid generation values");
        }

        for (double gene : chromosome.genes())
        {
            if (!isfinite(gene) ||
                gene < spec.lower ||
                gene > spec.upper)
            {
                throw invalid_argument(
                    "Gene is outside FloatSpec bounds");
            }
        }

        vector<pair<size_t, double>> replacements;

        for (size_t i = 0; i < chromosome.size(); ++i)
        {
            if (!rng.chance(gene_mutation_probability))
                continue;

            double x = chromosome.genes()[i];
            double direction = rng.next_real(0.0, 1.0);

            double y;

            if (direction <= 0.5)
                y = x - spec.lower;
            else
                y = spec.upper - x;

            double r = rng.next_real(0.0, 1.0);

            double exponent = pow(
                1.0 - static_cast<double>(generation) /
                max_generations,
                shape_parameter);

            double delta_t = y * (1.0 - pow(r, exponent));

            double new_value;

            if (direction <= 0.5)
                new_value = x - delta_t;
            else
                new_value = x + delta_t;

            if (new_value != x)
                replacements.emplace_back(i, new_value);
        }

        if (replacements.empty())
            return;

        vector<double>& genes = chromosome.mutable_genes();

        for (const auto& [index, value] : replacements)
            genes[index] = value;
    }
}
