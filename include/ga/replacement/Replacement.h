//
// Created by hazem on 10/19/26.
//
// Member 2 works here

#ifndef GENETIC_ALGORITHM_TOOLBOX_REPLACEMENT_H
#define GENETIC_ALGORITHM_TOOLBOX_REPLACEMENT_H
#include <cstddef>
#include <vector>

namespace ga {

    template <class C>
    class Replacement {
    public:
        virtual ~Replacement() = default;

        virtual std::vector<C> replace(
                const std::vector<C>& population,
                const std::vector<C>& offspring,
                std::size_t populationSize
        ) const = 0;
    };

}

#endif //GENETIC_ALGORITHM_TOOLBOX_REPLACEMENT_H