//
// Created by hazem on 10/19/26.
//

#ifndef GENETIC_ALGORITHM_TOOLBOX_CVRPPROBLEM_H
#define GENETIC_ALGORITHM_TOOLBOX_CVRPPROBLEM_H

#include "Problem.h"
#include "../include/ga/chromosomes/IntegerChromosome.h"

class CVRPProblem : public Problem<ga::IntegerChromosome> {
private:
    std::vector<double> px;                  
    std::vector<double> py;                  
    std::vector<double> demand;              
    std::vector<double> capacity;            
    std::vector<std::vector<double>> dist;  
    double bigM;                             
    double penaltyPerUnit;                   

    int customers() const;
    int trucks() const;
    void buildDistances();
    bool isPermutation(const std::vector<int>& genes) const;
    std::vector<std::vector<int> > decode(const std::vector<int>& genes) const;
    double routeDistance(const std::vector<int>& route) const;
    double routeLoad(const std::vector<int>& route) const;
    double totalOverload(const std::vector<std::vector<int> >& routes) const;

    
    double costOf(const std::vector<int>& genes) const;
    bool feasibleOf(const std::vector<int>& genes) const;
    void printGenes(const std::vector<int>& genes) const;

public:
    CVRPProblem();

    std::string name() const;
    void readInputs();
    bool validate() const;
    double fitness(const ga::IntegerChromosome& chromosome) const;
    bool isFeasible(const ga::IntegerChromosome& chromosome) const;
    bool isMinimization() const;
    ga::IntegerChromosome randomChromosome(ga::Random& rng) const;
    CompatibleOperators compatibleOperators() const;
    void printSolution(const ga::IntegerChromosome& chromosome) const;
};


#endif //GENETIC_ALGORITHM_TOOLBOX_CVRPPROBLEM_H