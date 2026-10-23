#include "CVRPProblem.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

const double EPS = 1e-9;

CVRPProblem::CVRPProblem() {
    bigM = 0;
    penaltyPerUnit = 0;
}

string CVRPProblem::name() const {
    return "Capacitated Vehicle Routing Problem (CVRP)";
}

// ------------------------------------------------------------ the menu

void CVRPProblem::readInputs() {
    px.clear();
    py.clear();
    demand.clear();
    capacity.clear();
    cache.clear();

    cout << "=== CVRP inputs ===\n";

    double x = 0, y = 0;
    cout << "Depot location (x y): ";
    cin >> x >> y;
    px.push_back(x);
    py.push_back(y);
    demand.push_back(0);

    int n = 0;
    cout << "Number of customers: ";
    cin >> n;
    for (int i = 1; i <= n; i++) {
        double d = 0;
        cout << "Customer " << i << " (x y demand): ";
        cin >> x >> y >> d;
        px.push_back(x);
        py.push_back(y);
        demand.push_back(d);
    }

    int k = 0;
    cout << "Number of trucks: ";
    cin >> k;

    int same = 0;
    cout << "Do all trucks have the same capacity? (1 = yes, 2 = no): ";
    cin >> same;
    if (same == 1) {
        double c = 0;
        cout << "Truck capacity: ";
        cin >> c;
        for (int t = 0; t < k; t++) capacity.push_back(c);
    } else {
        for (int t = 0; t < k; t++) {
            double c = 0;
            cout << "Capacity of truck " << t << ": ";
            cin >> c;
            capacity.push_back(c);
        }
    }

    buildDistances();
}

// ----------------------------------------------------------- the checks

bool CVRPProblem::validate() const {
    if (customers() < 1 || trucks() < 1) {
        cout << "No valid data was entered.\n";
        return false;
    }

    double totalDemand = 0, maxDemand = 0;
    for (int i = 1; i <= customers(); i++) {
        totalDemand += demand[i];
        maxDemand = max(maxDemand, demand[i]);
    }
    double totalCapacity = 0, maxCapacity = 0;
    for (int t = 0; t < trucks(); t++) {
        totalCapacity += capacity[t];
        maxCapacity = max(maxCapacity, capacity[t]);
    }

    bool ok = true;
    if (maxDemand > maxCapacity + EPS) {
        cout << "Impossible: a customer needs " << maxDemand
             << " but the biggest truck carries only " << maxCapacity << ".\n";
        ok = false;
    }
    if (totalDemand > totalCapacity + EPS) {
        cout << "Impossible: total demand " << totalDemand
             << " is bigger than total capacity " << totalCapacity << ".\n";
        ok = false;
    }
    return ok;
}

// ---------------------------------------------- fitness and feasibility


double CVRPProblem::costOf(const vector<int>& genes) const {
    if (!isPermutation(genes)){
        return 10 * bigM; 
    }

    vector<vector<int> > routes = decode(genes);
    double distance = 0;
    for (int t = 0; t < (int)routes.size(); t++) {
        distance += routeDistance(routes[t]);
    }

    double overload = totalOverload(routes);
    if (overload > EPS){
         return distance + bigM + penaltyPerUnit * overload;
    }
    return distance;
}

bool CVRPProblem::feasibleOf(const vector<int>& genes) const {
    if (!isPermutation(genes)){
         return false;
    }
    return totalOverload(decode(genes)) <= EPS;
}

bool CVRPProblem::isMinimization() const {
    return true;
}

double CVRPProblem::fitness(const ga::IntegerChromosome& chromosome) const {
    return costOf(chromosome.genes());
}

bool CVRPProblem::isFeasible(const ga::IntegerChromosome& chromosome) const {
    return feasibleOf(chromosome.genes());
}

void CVRPProblem::printSolution(const ga::IntegerChromosome& chromosome) const {
    printGenes(chromosome.genes());
}

ga::IntegerChromosome CVRPProblem::randomChromosome(ga::Random& rng) const {
    ga::IntegerSpec spec;
    spec.length = customers();
    spec.lower = 1;
    spec.upper = customers();
    spec.permutation = true;
    return ga::IntegerChromosome(spec, rng);
}

CompatibleOperators CVRPProblem::compatibleOperators() const {
    CompatibleOperators ops;
    ops.selections.push_back("roulette");
    ops.selections.push_back("tournament");
    ops.crossovers.push_back("order_crossover");
    ops.mutations.push_back("swap_mutation");
    ops.replacements.push_back("generational");
    ops.replacements.push_back("steady_state");
    return ops;
}

// -------------------------------------------------------------- printing

void CVRPProblem::printGenes(const vector<int>& genes) const {
    if (!isPermutation(genes)) {
        cout << "Invalid chromosome.\n";
        return;
    }
    vector<vector<int>> routes = decode(genes);
    cout << fixed << setprecision(2);

    double totalDistance = 0, totalLoad = 0;
    for (int t = 0; t < (int)routes.size(); t++) {
        if (routes[t].empty()){
             continue; 
        }

        cout << "Route for vehicle " << t << ":\n 0";
        for (int i = 0; i < (int)routes[t].size(); i++) cout << " -> " << routes[t][i];
        cout << " -> 0\n";

        double d = routeDistance(routes[t]);
        double l = routeLoad(routes[t]);
        cout << "Distance of the route: " << d << "\n";
        cout << "Load of the route: " << l << " (capacity " << capacity[t] << ")\n\n";
        totalDistance += d;
        totalLoad += l;
    }
    cout << "Total distance of all routes: " << totalDistance << "\n";
    cout << "Total load of all routes: " << totalLoad << "\n";

    double overload = totalOverload(routes);
    if (overload > EPS) cout << "WARNING: infeasible, total overload = " << overload << "\n";
}

// ------------------------------------------------------------- internals

int CVRPProblem::customers() const {
    if (px.empty()) return 0;
    return (int)px.size() - 1;
}

int CVRPProblem::trucks() const {
    return (int)capacity.size();
}

void CVRPProblem::buildDistances() {
    int m = (int)px.size();
    dist.assign(m, vector<double>(m, 0));
    double maxDist = 0;
    for (int a = 0; a < m; a++) {
        for (int b = 0; b < m; b++) {
            double dx = px[a] - px[b];
            double dy = py[a] - py[b];
            // Manhattan distance
            dist[a][b] = fabs(dx) + fabs(dy);
            // Euclidean distance
            //dist[b][a] = sqrt(dx * dx + dy * dy);
            maxDist = max(maxDist, dist[a][b]);
        }
    }
    bigM = (customers() + trucks()) * maxDist + 1;
    penaltyPerUnit = maxDist + 1;
}

bool CVRPProblem::isPermutation(const vector<int>& genes) const {
    int n = customers();
    if ((int)genes.size() != n) return false;
    vector<bool> seen(n + 1, false);
    for (int i = 0; i < n; i++) {
        int c = genes[i];
        if (c < 1 || c > n || seen[c]) return false;
        seen[c] = true;
    }
    return true;
}
vector<vector<int> > CVRPProblem::decode(const vector<int>& genes) const {
    map<vector<int>, vector<vector<int> > >::const_iterator it = cache.find(genes);
    if (it != cache.end()){
        return it->second;
    }
    vector<vector<int> > routes = buildRoutes(genes);
    cache[genes] = routes;
    return routes;
}

double CVRPProblem::bestSplit(const vector<int>& genes, int truck, int pos,
                              vector<vector<double> >& memo, vector<vector<int> >& choice) const {
    int n = (int)genes.size();
    if (pos == n){
         return 0;                    
    }
    if (truck == trucks()){
         return 1e18;       
    }
    if (memo[truck][pos] >= 0){
         return memo[truck][pos];   
    }

    double best = 1e18;
    int bestEnd = pos;
    double load = 0, distance_built_till_now = dist[0][genes[pos]];
    for (int end = pos + 1; end <= n; end++) {
        load += demand[genes[end - 1]];
        if (load > capacity[truck] + EPS){
            break;                
        }
        if (end - 1 > pos){
             distance_built_till_now += dist[genes[end - 2]][genes[end - 1]];
        }
        double total = distance_built_till_now + dist[genes[end - 1]][0] + bestSplit(genes, truck + 1, end, memo, choice);
        if (total < best){
            best = total;
            bestEnd = end;
        }
    }

    double skip = bestSplit(genes, truck + 1, pos, memo, choice);
    if (skip < best - EPS) { best = skip; bestEnd = pos; }

    choice[truck][pos] = bestEnd;
    memo[truck][pos] = best;
    return best;
}

vector<vector<int>> CVRPProblem::buildRoutes(const vector<int>& genes) const {
    int k = trucks();
    int n = (int)genes.size();

    vector<vector<double> > memo(k, vector<double>(n + 1, -1));
    vector<vector<int> > choice(k, vector<int>(n + 1, 0));
    vector<vector<int> > routes(k);
    if (bestSplit(genes, 0, 0, memo, choice) < 1e17) {
        int pos = 0;
        for (int t = 0; t < k && pos < n; t++) {
            int end = choice[t][pos];
            for (int i = pos; i < end; i++) routes[t].push_back(genes[i]);
            pos = end;
        }
        return routes;
    }
    else{
        vector<vector<int>> routes(k);
        vector<double> remaining = capacity;   
        int truck = 0;                         
        for (int i = 0; i < (int)genes.size(); i++) {
            int customer = genes[i];
            if (demand[customer] > remaining[truck] + EPS && !routes[truck].empty() && truck < k - 1) {
                truck++;
            }
            routes[truck].push_back(customer);
            remaining[truck] -= demand[customer];
        }
        return routes;
    }
}


double CVRPProblem::routeDistance(const vector<int>& route) const {
    if (route.empty()){
        return 0;
    }
    double total = dist[0][route[0]];
    for (int i = 0; i + 1 < (int)route.size(); i++){
        total += dist[route[i]][route[i + 1]];
    }
    total += dist[route[route.size() - 1]][0];
    return total;
}

double CVRPProblem::routeLoad(const vector<int>& route) const {
    double load = 0;
    for (int i = 0; i < (int)route.size(); i++){
        load += demand[route[i]];
    }
    return load;
}

double CVRPProblem::totalOverload(const vector<vector<int> >& routes) const {
    double over = 0;
    for (int t = 0; t < (int)routes.size(); t++) {
        over += max(0.0, routeLoad(routes[t]) - capacity[t]);
    }
    return over;
}