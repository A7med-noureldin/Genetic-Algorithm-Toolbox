//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_GACONFIG_H
#define GENETIC_ALGORITHM_TOOLBOX_GACONFIG_H

#include <string>

namespace ga
{
    //a plain struct with public members and default values written right on each member.
    //No constructor needed. Users create one and change what they want.
    struct GAConfig
    {
        int population_size = 100;
        int max_generations = 500;
        double crossover_rate = 0.8;
        double mutation_rate = 0.05;   // per chromosome
        unsigned int seed = 42;
        bool minimize = false;
    };

    // Returns an empty string if the config is valid, otherwise a message saying what is wrong.
    inline std::string validate(const GAConfig& ga_config)
    {
        if (ga_config.population_size < 2)
            return "population_size must be at least 2";
        if (ga_config.max_generations < 1)
            return "max_generations must be at least 1";
        if (ga_config.crossover_rate < 0.0 || ga_config.crossover_rate > 1.0)
            return "crossover_rate must be between 0 and 1";
        if (ga_config.mutation_rate < 0.0 || ga_config.mutation_rate > 1.0)
            return "mutation_rate must be between 0 and 1";
        return "";
    }
}

#endif //GENETIC_ALGORITHM_TOOLBOX_GACONFIG_H