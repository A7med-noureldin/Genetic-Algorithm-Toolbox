#include "operatorCatalog.h"

#include <algorithm>
#include <numeric>
#include <random>

#include "../selection/RouletteWheelSelection.h"
#include "../selection/TournamentSelection.h"
#include "../crossover/OrderCrossover.h"
#include "../crossover/MultiPointCrossover.h"
#include "../mutation/SwapMutation.h"
#include "../replacement/GenerationalReplacement.h"
#include "../replacement/SteadyStateReplacement.h"

using namespace std;

typedef ga::IntegerChromosome IC;

static OperatorOption plainOption(const string& id, const string& name) {
    OperatorOption o;
    o.id = id;
    o.name = name;
    return o;
}

static OperatorOption paramOption(const string& id, const string& name, const string& paramName,
                                  bool isInteger, double low, double high, double def) {
    OperatorOption o = plainOption(id, name);
    o.paramName = paramName;
    o.paramIsInteger = isInteger;
    o.paramMin = low;
    o.paramMax = high;
    o.paramDefault = def;
    return o;
}

// ----------------------------------------------------------- the lists

vector<OperatorOption> OperatorCatalog<IC>::selections() {
    vector<OperatorOption> list;
    list.push_back(plainOption("roulette", "Roulette wheel"));
    list.push_back(paramOption("tournament", "Tournament", "Tournament size (how many compete each time)",
                               true, 1, 1000, 3));
    return list;
}

vector<OperatorOption> OperatorCatalog<IC>::crossovers() {
    vector<OperatorOption> list;
    list.push_back(plainOption("order_crossover", "Order crossover"));
    list.push_back(paramOption("multi_point_crossover", "Multi-point crossover", "Number of cut points",
                               true, 1, 100, 2));
    return list;
}

vector<OperatorOption> OperatorCatalog<IC>::mutations() {
    vector<OperatorOption> list;
    list.push_back(paramOption("swap_mutation", "Swap mutation", "Mutation probability per gene",
                               false, 0.001, 0.1, 0.05));
    return list;
}

vector<OperatorOption> OperatorCatalog<IC>::replacements() {
    vector<OperatorOption> list;
    list.push_back(plainOption("generational", "Generational (children replace the whole population)"));
    list.push_back(paramOption("steady_state", "Steady state (the best children replace the worst chromosomes)",
                               "How many chromosomes to replace each generation", true, 1, -1, 2));
    return list;
}

// ------------------------------------------------------- the factories

ga::Selection<IC>* OperatorCatalog<IC>::makeSelection(const string& id, double param) {
    if (id == "roulette") return new ga::RouletteWheelSelection<IC>();
    if (id == "tournament") return new ga::TournamentSelection<IC>((size_t)param);
    return 0;
}

ga::Crossover<int>* OperatorCatalog<IC>::makeCrossover(const string& id, double param) {
    if (id == "order_crossover") return new ga::OrderCrossover<int>();
    if (id == "multi_point_crossover") return new ga::MultiPointCrossover<int>((int)param);
    return 0;
}

ga::Mutation<IC>* OperatorCatalog<IC>::makeMutation(const string& id, double param) {
    if (id == "swap_mutation") return new ga::SwapMutation(param);
    return 0;
}

ga::Replacement<IC>* OperatorCatalog<IC>::makeReplacement(const string& id, double param,
                                                          const ga::GAConfig& config) {
    if (id == "generational") return new ga::GenerationalReplacement<IC>();
    if (id == "steady_state") return new ga::SteadyStateReplacement<IC>(config, (size_t)param);
    return 0;
}