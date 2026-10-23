//
// Created by hazem on 10/19/26.
//
// OneMax: evolve a bit string towards all ones, using the toolbox operators directly.

#include <iostream>
#include <vector>

#include "../include/ga/core/GAConfig.h"
#include "../include/ga/core/Random.h"
#include "../include/ga/chromosomes/BinaryChromosome.h"
#include "../include/ga/selection/TournamentSelection.h"
#include "../include/ga/crossover/MultiPointCrossover.h"
#include "../include/ga/mutation/FlipMutation.h"
#include "../include/ga/replacement/GenerationalReplacement.h"

using namespace std;

static double countOnes(const ga::BinaryChromosome& c) {
    double ones = 0;
    for (size_t i = 0; i < c.size(); i++) ones += c[i];
    return ones;
}

int main() {
    ga::GAConfig config;
    config.population_size = 50;
    config.max_generations = 100;
    const int length = 40;

    ga::Random rng(config.seed);
    ga::BinarySpec spec;
    spec.length = length;

    ga::TournamentSelection<ga::BinaryChromosome> selection(3);
    ga::MultiPointCrossover<uint8_t> crossover(2);
    ga::FlipMutation mutation(1.0 / length);
    ga::GenerationalReplacement<ga::BinaryChromosome> replacement;

    vector<ga::BinaryChromosome> population;
    for (int i = 0; i < config.population_size; i++) {
        population.push_back(ga::BinaryChromosome(spec, rng));
        population.back().set_fitness(countOnes(population.back()));
    }

    for (int g = 0; g < config.max_generations; g++) {
        vector<size_t> picked = selection.select(population, population.size(), rng);

        vector<ga::BinaryChromosome> children;
        for (size_t i = 0; i + 1 < picked.size(); i += 2) {
            const ga::BinaryChromosome& p1 = population[picked[i]];
            const ga::BinaryChromosome& p2 = population[picked[i + 1]];
            ga::BinaryChromosome c1(p1.genes()), c2(p2.genes());
            if (rng.chance(config.crossover_rate)) {
                ga::Crossover<uint8_t>::offspring pair = crossover.crossover(p1, p2);
                c1 = ga::BinaryChromosome(pair.first.genes());
                c2 = ga::BinaryChromosome(pair.second.genes());
            }
            children.push_back(c1);
            children.push_back(c2);
        }
        for (size_t i = 0; i < children.size(); i++) {
            mutation.mutate(children[i], rng, g, config.max_generations);
            children[i].set_fitness(countOnes(children[i]));
        }

        population = replacement.replace(population, children, population.size());
    }

    double best = 0;
    for (size_t i = 0; i < population.size(); i++) best = max(best, population[i].get_fitness());
    cout << "Best OneMax fitness: " << best << " / " << length << "\n";
    return 0;
}
