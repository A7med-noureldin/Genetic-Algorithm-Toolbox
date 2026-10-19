//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_RANDOM_H
#define GENETIC_ALGORITHM_TOOLBOX_RANDOM_H

#include <random>
#include <algorithm>
#include <vector>

namespace ga
{
    class Random
    {
    private:
        unsigned int seed;
        std::mt19937 gen;
    public:
        Random() : seed(std::random_device{}()), gen(seed) {}
        explicit Random(unsigned int s) : seed(s), gen(s) {}

        // deleting the copy operator so no one copies (pass by value) by mistake because both will give the same random seed
        // don't forget to allways pass it by reference to operators: Random&
        Random(const Random&) = delete;
        Random& operator=(const Random&) = delete;

        //helpers so operators never build their own distributions
        int next_int(int lo, int hi)                 // both ends inclusive
        {
            std::uniform_int_distribution<int> dist(lo, hi);
            return dist(gen);
        }

        double next_real(double lo, double hi)
        {
            std::uniform_real_distribution<double> dist(lo, hi);
            return dist(gen);
        }

        bool chance(double p)                        // true with probability p (crossover rate, mutation rate)
        {
            return next_real(0.0, 1.0) < p;
        }

        template <class T>
        void shuffle(std::vector<T>& v)              // permutation init, shuffle-type mutations
        {
                std::shuffle(v.begin(), v.end(), gen);
        }

        std::mt19937& engine() { return gen; }       // raw engine for anything unusual
        unsigned int get_seed() const { return seed; }  // handy for printing which seed produced a result

    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_RANDOM_H