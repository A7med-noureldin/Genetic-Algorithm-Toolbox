#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>

#include "../include/ga/core/Random.h"

class BaseProblem {
public:

    virtual std::string name() const = 0;

    virtual void readInputs() = 0;

    virtual bool validate() const = 0;

    virtual ~BaseProblem() {}
};


struct CompatibleOperators {
    std::vector<std::string> selections;
    std::vector<std::string> crossovers;
    std::vector<std::string> mutations;
    std::vector<std::string> replacements;
};

template <class C>
class Problem : public BaseProblem {
public:
    
    virtual double fitness(const C& chromosome) const = 0;
    virtual bool isFeasible(const C& chromosome) const = 0;

    
    virtual bool isMinimization() const = 0;

    
    virtual C randomChromosome(ga::Random& rng) const = 0;

    
    virtual CompatibleOperators compatibleOperators() const = 0;

    
    virtual void printSolution(const C& chromosome) const = 0;
};

#endif