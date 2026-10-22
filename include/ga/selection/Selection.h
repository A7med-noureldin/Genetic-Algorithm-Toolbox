//
// Created by hazem on 10/19/26.
//

//Member 2 Works here

#ifndef GENETIC_ALGORITHM_TOOLBOX_SELECTION_H
#define GENETIC_ALGORITHM_TOOLBOX_SELECTION_H

#include <cstddef>
#include <vector>
#include "../core/Random.h"

namespace ga{
    template <class C>
    class Selection {
    public:
        virtual ~Selection() = default;

        virtual std::vector<std::size_t> select(
                const std::vector<C>& population,
                std::size_t count,
                Random& randomGenerator
        ) const = 0;
    };
}

#endif //GENETIC_ALGORITHM_TOOLBOX_SELECTION_H