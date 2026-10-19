//
// Created by hazem on 10/19/26.
//
#ifndef GENETIC_ALGORITHM_TOOLBOX_CHROMOSOME_H
#define GENETIC_ALGORITHM_TOOLBOX_CHROMOSOME_H
#include <vector>
#include <cstddef>
#include <utility>

namespace ga
{
    template <class T>
    class Chromosome
    {
    protected:
        std::vector<T> gen;
        double fitness_value = 0.0;
        bool evaluated = false;                        //so you never recompute fitness for an unchanged chromosome
        bool feasible = true;
    public:
        Chromosome() {}
        explicit Chromosome(std::vector<T> vec) : gen(std::move(vec)) {}
        virtual ~Chromosome() = default;
        Chromosome(const Chromosome&) = default;
        Chromosome(Chromosome&&) = default;
        Chromosome& operator=(const Chromosome&) = default;
        Chromosome& operator=(Chromosome&&) = default;

        const std::vector<T>& genes() const { return gen; }
        std::size_t size() const { return gen.size(); }

        std::vector<T>& mutable_genes()
        {
            evaluated = false;
            return gen;
        }
        double get_fitness() const { return fitness_value; }
        void set_fitness(double f)             // called by the engine after evaluation
        {
                fitness_value = f;
                evaluated = true;
        }
        bool is_evaluated() const { return evaluated; }
        bool is_feasible() const { return feasible; }
        void set_feasible(bool f) { feasible = f; }
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_CHROMOSOME_H