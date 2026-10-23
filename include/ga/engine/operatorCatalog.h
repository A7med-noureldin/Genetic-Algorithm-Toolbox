#ifndef OPERATOR_CATALOG_H
#define OPERATOR_CATALOG_H

#include <string>
#include <vector>

#include "../chromosomes/IntegerChromosome.h"
#include "../core/GAConfig.h"
#include "../selection/Selection.h"
#include "../crossover/Crossover.h"
#include "../mutation/Mutation.h"
#include "../replacement/Replacement.h"

struct OperatorOption {
    std::string id;           
    std::string name;         
    std::string paramName;    
    bool paramIsInteger;
    double paramMin;
    double paramMax;          
    double paramDefault;

    OperatorOption() : paramIsInteger(false), paramMin(0), paramMax(0), paramDefault(0) {}
};


template <class C>
struct OperatorCatalog;

template <>
struct OperatorCatalog<ga::IntegerChromosome> {
    typedef int Gene;    

    static std::vector<OperatorOption> selections();
    static std::vector<OperatorOption> crossovers();
    static std::vector<OperatorOption> mutations();
    static std::vector<OperatorOption> replacements();

    static ga::Selection<ga::IntegerChromosome>* makeSelection(const std::string& id, double param);
    static ga::Crossover<int>* makeCrossover(const std::string& id, double param);
    static ga::Mutation<ga::IntegerChromosome>* makeMutation(const std::string& id, double param);
    static ga::Replacement<ga::IntegerChromosome>* makeReplacement(const std::string& id, double param,
                                                                   const ga::GAConfig& config);
};

#endif