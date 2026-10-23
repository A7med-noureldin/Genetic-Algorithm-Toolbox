#ifndef ENGINE_H
#define ENGINE_H

#include <cfloat>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../core/GAConfig.h"
#include "../core/Random.h"
#include "../../../cvrp/Problem.h"
#include "operatorCatalog.h"

template <class C>
class GAEngine {
public:
    typedef typename OperatorCatalog<C>::Gene Gene;

    GAEngine();
    ~GAEngine();

    bool askSettings(const Problem<C>& problem);   
    void printSettings() const;

    void setProblem(Problem<C>* p);                
    bool run();                                    

    const C& best() const;
    double bestCost() const;
    bool bestIsFeasible() const;

private:
    ga::GAConfig config;       

    OperatorOption selectionOption;     double selectionParam;
    OperatorOption crossoverOption;     double crossoverParam;
    OperatorOption mutationOption;      double mutationParam;
    OperatorOption replacementOption;   double replacementParam;

    Problem<C>* problem;
    ga::Random* rng;
    ga::Selection<C>* selection;
    ga::Crossover<Gene>* crossover;
    ga::Mutation<C>* mutation;
    ga::Replacement<C>* replacement;

    C bestSoFar;
    double bestCostSoFar;
    bool haveBest;

    bool chooseOption(const std::string& title, const std::vector<OperatorOption>& options,
                      OperatorOption& chosen, double& param);
    std::vector<OperatorOption> onlyAllowed(const std::vector<OperatorOption>& options,
                                            const std::vector<std::string>& allowedIds) const;
    void printOption(const char* label, const OperatorOption& option, double param) const;

    void createOperators();
    void deleteOperators();
    double evaluate(C& c);
    std::vector<C> makeChildren(const std::vector<C>& population, int generation);


};


template <class C>
GAEngine<C>::GAEngine() {
    selectionParam = 0;
    crossoverParam = 0;
    mutationParam = 0;
    replacementParam = 0;
    problem = 0;
    rng = 0;
    selection = 0;
    crossover = 0;
    mutation = 0;
    replacement = 0;
    bestCostSoFar = DBL_MAX;
    haveBest = false;
}

template <class C>
GAEngine<C>::~GAEngine() {
    deleteOperators();
    delete rng;
}

// =====================================================================
//                                THE MENU
// =====================================================================

template <class C>
bool GAEngine<C>::askSettings(const Problem<C>& p) {
    std::cout << "\n=== Genetic algorithm settings ===\n";
    std::cout << "(press Enter to use the value shown in [ ])\n\n";

    while (std::cin.peek() == ' ' || std::cin.peek() == '\t' || std::cin.peek() == '\r') std::cin.get();
    if (std::cin.peek() == '\n') std::cin.get();

    while (true) {
        std::cout << "Population size (2 to 100000) [" << config.population_size << "]: ";
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) return false;

        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) break;                       

        std::istringstream number(word);
        int v = 0;
        char extra;
        if ((number >> v) && !(number >> extra) && v >= 2 && v <= 100000) {
            config.population_size = v;
            break;
        }
        std::cout << "Invalid, try again.\n";
    }

    while (true) {
        std::cout << "Number of generations (1 to 1000000) [" << config.max_generations << "]: ";
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) return false;

        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) break;

        std::istringstream number(word);
        int v = 0;
        char extra;
        if ((number >> v) && !(number >> extra) && v >= 1 && v <= 1000000) {
            config.max_generations = v;
            break;
        }
        std::cout << "Invalid, try again.\n";
    }

    while (true) {
        std::cout << "Crossover probability Pc (0 to 1) [" << config.crossover_rate << "]: ";
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) return false;

        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) break;

        std::istringstream number(word);
        double v = 0;
        char extra;
        if ((number >> v) && !(number >> extra) && v >= 0.0 && v <= 1.0) {
            config.crossover_rate = v;
            break;
        }
        std::cout << "Invalid, try again.\n";
    }

    CompatibleOperators allowed = p.compatibleOperators();

    if (!chooseOption("Selection method",
                      onlyAllowed(OperatorCatalog<C>::selections(), allowed.selections),
                      selectionOption, selectionParam)) return false;

    if (!chooseOption("Crossover method",
                      onlyAllowed(OperatorCatalog<C>::crossovers(), allowed.crossovers),
                      crossoverOption, crossoverParam)) return false;

    if (!chooseOption("Mutation method",
                      onlyAllowed(OperatorCatalog<C>::mutations(), allowed.mutations),
                      mutationOption, mutationParam)) return false;

    if (!chooseOption("Replacement method",
                      onlyAllowed(OperatorCatalog<C>::replacements(), allowed.replacements),
                      replacementOption, replacementParam)) return false;

    return true;
}

template <class C>
std::vector<OperatorOption> GAEngine<C>::onlyAllowed(const std::vector<OperatorOption>& options,
                                                     const std::vector<std::string>& allowedIds) const {
    std::vector<OperatorOption> result;
    for (size_t i = 0; i < options.size(); i++) {
        for (size_t j = 0; j < allowedIds.size(); j++) {
            if (options[i].id == allowedIds[j]) {
                result.push_back(options[i]);
                break;
            }
        }
    }
    return result;
}

template <class C>
bool GAEngine<C>::chooseOption(const std::string& title, const std::vector<OperatorOption>& options,
                               OperatorOption& chosen, double& param) {
    if (options.empty()) {
        std::cout << "\nThis problem accepts no operator for: " << title << "\n";
        return false;
    }

    std::cout << "\n" << title << ":\n";
    for (int i = 0; i < options.size(); i++) {
        std::cout << "  " << i + 1 << ") " << options[i].name << "\n";
    }

    int index = 0;
    while (true) {
        std::cout << "Your choice (1 to " << options.size() << ") [1]: ";
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) return false;

        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) break;                       

        std::istringstream number(word);
        int v = 0;
        char extra;
        if ((number >> v) && !(number >> extra) && v >= 1 && v <= (int)options.size()) {
            index = (size_t)(v - 1);
            break;
        }
        std::cout << "Invalid, try again.\n";
    }
    chosen = options[index];

    if (chosen.paramName.empty()) return true;             

    double low = chosen.paramMin;
    double high = chosen.paramMax;
    if (high < 0) high = config.population_size;           
    param = chosen.paramDefault;
    if (param > high) param = high;

    while (true) {
        std::cout << chosen.paramName << " (" << low << " to " << high << ") [" << param << "]: ";
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) return false;

        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) break;                       

        std::istringstream number(word);
        double v = 0;
        char extra;
        bool ok = (number >> v) && !(number >> extra) && v >= low && v <= high;
        if (ok && chosen.paramIsInteger && v != std::floor(v)) ok = false;
        if (ok) {
            param = v;
            break;
        }
        std::cout << "Invalid, try again.\n";
    }
    return true;
}

template <class C>
void GAEngine<C>::printOption(const char* label, const OperatorOption& option, double param) const {
    std::cout << label << option.name;
    if (!option.paramName.empty()) std::cout << "  (" << option.paramName << " = " << param << ")";
    std::cout << "\n";
}

template <class C>
void GAEngine<C>::printSettings() const {
    std::cout << "\n--- Chosen settings ---\n";
    printOption("Selection:   ", selectionOption, selectionParam);
    printOption("Crossover:   ", crossoverOption, crossoverParam);
    printOption("Mutation:    ", mutationOption, mutationParam);
    printOption("Replacement: ", replacementOption, replacementParam);
    std::cout << "Crossover probability Pc: " << config.crossover_rate << "\n";
    std::cout << "Population:  " << config.population_size << "\n";
    std::cout << "Generations: " << config.max_generations << "\n";
}

// =====================================================================
//                                 THE RUN
// =====================================================================

template <class C>
void GAEngine<C>::setProblem(Problem<C>* p) {
    problem = p;
}

template <class C>
const C& GAEngine<C>::best() const { return bestSoFar; }

template <class C>
double GAEngine<C>::bestCost() const { return bestCostSoFar; }

template <class C>
bool GAEngine<C>::bestIsFeasible() const { return bestSoFar.is_feasible(); }

template <class C>
bool GAEngine<C>::run() {
    if (problem == 0) {
        std::cout << "No problem was given to the engine.\n";
        return false;
    }

    std::string error = ga::validate(config);
    if (!error.empty()) {
        std::cout << "Invalid settings: " << error << "\n";
        return false;
    }

    std::cout << "\n=== Running the genetic algorithm ===\n";
    delete rng;
    rng = new ga::Random();                    
    haveBest = false;
    bestCostSoFar = DBL_MAX;

    try {
        createOperators();

        std::vector<C> population;
        for (int i = 0; i < config.population_size; i++) {
            population.push_back(problem->randomChromosome(*rng));
        }
        for (size_t i = 0; i < population.size(); i++) evaluate(population[i]);

        int step = config.max_generations / 10;
        if (step < 1) step = 1;

        for (int g = 0; g < config.max_generations; g++) {
            std::vector<C> children = makeChildren(population, g);

            std::vector<C> next = replacement->replace(population, children, (size_t)config.population_size);
            population = next;

            if ((g + 1) % step == 0 || g + 1 == config.max_generations) {
                std::cout << "Generation " << (g + 1) << ": best cost so far = " << bestCostSoFar << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error while running: " << e.what() << "\n";
        return false;
    }
    return true;
}


template <class C>
void GAEngine<C>::createOperators() {
    deleteOperators();

    selection = OperatorCatalog<C>::makeSelection(selectionOption.id, selectionParam);
    crossover = OperatorCatalog<C>::makeCrossover(crossoverOption.id, crossoverParam);
    mutation = OperatorCatalog<C>::makeMutation(mutationOption.id, mutationParam);
    replacement = OperatorCatalog<C>::makeReplacement(replacementOption.id, replacementParam, config);

    if (selection == 0 || crossover == 0 || mutation == 0 || replacement == 0) {
        throw std::runtime_error("an operator could not be created (unknown id)");
    }
}

template <class C>
void GAEngine<C>::deleteOperators() {
    delete selection;
    delete crossover;
    delete mutation;
    delete replacement;
    selection = 0;
    crossover = 0;
    mutation = 0;
    replacement = 0;
}


template <class C>
double GAEngine<C>::evaluate(C& c) {
    double cost = problem->fitness(c);
    bool minimize = problem->isMinimization();

    c.set_fitness(minimize ? 1.0 / (1.0 + cost) : cost);
    c.set_feasible(problem->isFeasible(c));

    bool better = !haveBest || (minimize ? cost < bestCostSoFar : cost > bestCostSoFar);
    if (better) {
        bestSoFar = c;
        bestCostSoFar = cost;
        haveBest = true;
    }
    return cost;
}


template <class C>
std::vector<C> GAEngine<C>::makeChildren(const std::vector<C>& population, int generation) {
    std::vector<size_t> picked = selection->select(population, (size_t)config.population_size, *rng);

    std::vector<C> children;
    for (size_t i = 0; i < picked.size(); i += 2) {
        const C& p1 = population[picked[i]];
        size_t j = i + 1;
        if (j >= picked.size()) j = i;                 
        const C& p2 = population[picked[j]];

        C c1(p1.genes());
        C c2(p2.genes());

        if (rng->chance(config.crossover_rate)) {
            typename ga::Crossover<Gene>::offspring pair = crossover->crossover(p1, p2);
            c1 = C(pair.first.genes());
            c2 = C(pair.second.genes());
        }

        children.push_back(c1);
        if ((int)children.size() < config.population_size) children.push_back(c2);
    }

    for (size_t i = 0; i < children.size(); i++) {
        mutation->mutate(children[i], *rng, generation, config.max_generations);
        evaluate(children[i]);
    }
    return children;
}

#endif