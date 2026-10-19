//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_TYPES_H
#define GENETIC_ALGORITHM_TOOLBOX_TYPES_H
#include <functional>

namespace ga
{
    // The user's fitness function: takes a chromosome, returns a number.
    // Templated on the chromosome type so it works for Binary, Integer and Float.
    // The chromosome is passed by const reference, so the user's function can read it but not modify it.
    template <class C>
    using FitnessFunction = std::function<double(const C&)>;

    // The user's feasibility check: takes a chromosome, returns true if it is a legal solution.
    template <class C>
    using FeasibilityFunction = std::function<bool(const C&)>;
}

#endif //GENETIC_ALGORITHM_TOOLBOX_TYPES_H