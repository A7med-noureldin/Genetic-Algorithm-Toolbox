# Genetic Algorithm Toolbox + CVRP Solver

A modular, object-oriented Genetic Algorithm (GA) toolbox written in C++ (no GA libraries used), and a Capacitated Vehicle Routing Problem (CVRP) solver built on top of it.

---

## 1. Project Structure

```
Genetic-Algorithm-Toolbox/
├── include/ga/                # The reusable GA toolbox (Part 1)
│   ├── chromosomes/           # Binary, integer, floating-point chromosomes
│   ├── core/                  # Shared basics (random number generator, common types)
│   ├── selection/             # Roulette wheel, tournament
│   ├── crossover/             # Multi-point crossover, order crossover
│   ├── mutation/              # Mutation methods
│   ├── replacement/           # Generational, steady state
│   ├── engine/                # The GA engine (problem-independent)
│   └── ga.h                   # Single header that includes the whole toolbox
├── cvrp/                      # The CVRP problem (Part 2)
│   ├── Problem.h              # Interface every optimization problem implements
│   ├── CVRPProblem.h
│   └── CVRPProblem.cpp
├── examples/                  # Example inputs
├── tests/                     # Tests
├── presentation/              # Presentation files
├── main.cpp                   # Menu, settings input, runs the GA
├── CMakeLists.txt
└── README.md
```

To use the toolbox in your own project, include `ga/ga.h` and implement the `Problem` interface.

## 2. Build and Run

The project is built with CMake.

```bash
cmake -S . -B build
cmake --build build
./build/Genetic-Algorithm-Toolbox        # [FILL IN: executable name from CMakeLists.txt]
```

On Windows the executable is in `build\` (or `build\Debug\`) and ends in `.exe`.

Input can also be piped from a file:

```bash
./ga_toolbox < input.txt
```

---

## 3. Part 1: GA Toolbox

### 3.1 Components

| Requirement | Implemented                                                                    |
|---|--------------------------------------------------------------------------------|
| Chromosome representations (3) | Binary, Integer (including permutation mode via `IntegerSpec`), Floating-point |
| Selection methods (2) | `roulette` (roulette wheel), `tournament`                                      |
| Crossover | Multi-point crossover, `order_crossover`                                       |
| Mutation methods (3) | `swap_mutation`, `flip_mutation`, `uniform_mutation`, `non_uniform_mutation`     |
| Replacement methods (2) | `generational`, `steady_state`                                                 |

### 3.2 Design

The GA engine knows nothing about the problem it solves. A problem plugs in by implementing one interface:

```cpp
class Problem {
    virtual double fitness(const Chromosome&) const;      // evaluation function
    virtual bool   isFeasible(const Chromosome&) const;   // feasibility check
    virtual bool   isMinimization() const;
    virtual Chromosome randomChromosome(ga::Random&) const;
    virtual CompatibleOperators compatibleOperators() const;
    virtual void   printSolution(const Chromosome&) const;
};
```

- **Fitness function:** supplied by the user through `fitness()`.
- **Feasibility check:** supplied by the user through `isFeasible()`.
- **Compatible operators:** each problem lists which selection, crossover, mutation and replacement methods are valid for its chromosome type (for example, order crossover only makes sense for permutations). The menu only offers those.
- **Reuse:** to solve a new problem, write a new `Problem` subclass. The GA code is not touched.

### 3.3 Handling infeasible solutions

Infeasible chromosomes are handled with a **penalty method**. Their cost is increased by a large constant plus an amount proportional to how much the constraint is violated, so every infeasible solution is always worse than every feasible one, but a "less infeasible" solution is still better than a "more infeasible" one. This keeps useful genetic material in the population and gives the search a direction towards feasibility.

### 3.4 Hyperparameters and defaults

Every value can be changed by the user. Pressing Enter keeps the default.

| Parameter | Range | Default |
|---|---|---|
| Population size | 2 to 100000 | 100 |
| Number of generations | 1 to 1000000 | 500 |
| Crossover probability Pc | 0 to 1 | 0.8 |
| Selection method | roulette / tournament | roulette |
| Crossover method | order crossover | order crossover |
| Mutation method | swap mutation | swap mutation |
| Mutation probability per gene | 0.001 to 0.1 | 0.05 |
| Replacement method | generational / steady state | generational |

---

## 4. Part 2: CVRP

### 4.1 Problem

A fleet of trucks starts at a depot and must serve all customers. Each customer has a demand and each truck has a maximum capacity. The goal is to minimize the total distance traveled while no truck exceeds its capacity and every customer is served exactly once.

### 4.2 Encoding

A chromosome is a **permutation of the customers 1..n** (integer chromosome, permutation mode). Any CVRP solution can be written this way by concatenating the routes in truck order.

### 4.3 Decoding

The permutation is split into consecutive segments, one per truck, using a **dynamic programming split** (`bestSplit`):

- Each truck gets one segment, or none (empty trucks are allowed).
- Each truck's own capacity is respected, so different capacities per truck are supported.
- The DP picks the split with the minimum total distance.
- Results are cached (`MAX_CACHE_ENTRIES`) to avoid decoding the same permutation twice.
- If no feasible split exists, a greedy fallback fills trucks in order. The overload is then penalized in the fitness.

### 4.4 Fitness and feasibility

```
cost = total distance of all routes                           (if feasible)
cost = total distance + bigM + penaltyPerUnit * overload      (if infeasible)
cost = 10 * bigM                                              (if not a valid permutation)
```

- `bigM = (customers + trucks) * maxDist + 1`, an upper bound on the length of any feasible solution.
- A solution is feasible when the total overload over all trucks is zero.

### 4.5 Distance

Set at the top of `CVRPProblem.cpp`:

```cpp
const bool IS_EUCLIDEAN = true;   // true: rounded Euclidean (TSPLIB EUC_2D), false: Manhattan
```

Rounded Euclidean uses `floor(sqrt(dx*dx + dy*dy) + 0.5)`, which matches TSPLIB. Use `true` for the Augerat (A-n...) benchmark files.

### 4.6 Input format

```
=== Genetic Algorithm Toolbox ===

Choose a problem:
  1) Capacitated Vehicle Routing Problem (CVRP)
Your choice:1

=== CVRP inputs ===
Depot location (x y):4 4
Number of customers:16
Customer 1 (x y demand):228 0 1
...
Number of trucks:4
Do all trucks have the same capacity? (1 = yes, 2 = no):1
Truck capacity:15

Data accepted.

=== Genetic algorithm settings ===
(press Enter to use the value shown in [ ])
Population size (2 to 100000) [100]:
...
```

If trucks have different capacities, answer `2` and the program asks for the capacity of each truck.

When piping input from a file, write every setting value explicitly. Do not rely on blank lines, because trailing blank lines are easily lost.

### 4.7 Output format

```
Route for vehicle 0:
 0 -> 3 -> 7 -> 0
Distance of the route: 123.00
Load of the route: 14 (capacity 15)

...
Total distance of all routes: 456.00
Total load of all routes: 55
```

If the best solution found is infeasible, the program prints a warning with the total overload.

### 4.8 Validation

Before running, the program checks that:

- at least one customer and one truck exist,
- no customer demand exceeds the largest truck capacity,
- total demand does not exceed total capacity.

---