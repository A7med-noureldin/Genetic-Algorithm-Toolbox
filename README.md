# Genetic Algorithm Toolbox + CVRP

Soft Computing, Assignment 1 (Cairo University, FCAI).

A reusable genetic algorithm (GA) toolbox written from scratch in C++, plus a program that uses it to solve the Capacitated Vehicle Routing Problem (CVRP).

**Core rule:** the toolbox (`include/ga/`) must know nothing about CVRP. All CVRP knowledge lives in `cvrp/`. The only things CVRP gives the toolbox are a fitness function and a feasibility check function (plus its own decoder).

## Table of contents

1. [Team ownership](#1-team-ownership)
2. [Status board](#2-status-board)
3. [Project layout](#3-project-layout)
4. [What every file does](#4-what-every-file-does)
5. [How the pieces connect](#5-how-the-pieces-connect)
6. [Decisions we have made (rules)](#6-decisions-we-have-made-rules)
7. [Chromosome rules for operator authors](#7-chromosome-rules-for-operator-authors)
8. [Operator contracts](#8-operator-contracts)
9. [Build and run](#9-build-and-run)
10. [Testing](#10-testing)
11. [Git workflow](#11-git-workflow)
12. [Code conventions](#12-code-conventions)
13. [Adding a new operator](#13-adding-a-new-operator)
14. [Order of work and milestones](#14-order-of-work-and-milestones)
15. [Still undecided](#15-still-undecided)
16. [Design justification (for the discussion)](#16-design-justification-for-the-discussion)
17. [CVRP representation FAQ](#17-cvrp-representation-faq)
18. [Glossary](#18-glossary)
19. [Grading checklist](#19-grading-checklist)
20. [Submission](#20-submission)

---

## 1. Team ownership

| Member | Area | Where you work |
|---|---|---|
| **Member 1** | Toolbox structure, interfaces, config, RNG, 3 chromosome representations | `include/ga/core/`, `include/ga/chromosomes/`, all interface headers, `ga.h`, CMake, README |
| **Member 2** | 2 selection methods, 2 replacement methods | `include/ga/selection/`, `include/ga/replacement/` |
| **Member 3** | Multi-point crossover, Order crossover (OX) | `include/ga/crossover/` |
| **Member 4** | 3 mutation methods, constraint handling for infeasible solutions | `include/ga/mutation/`, `include/ga/constraints/` |
| **Member 5** | CVRP modeling, decoder, fitness, GA engine integration, final testing | `cvrp/`, `main.cpp`, `data/` |

Collaboration:
- Member 2 helps Member 5 with testing and integration once selection and replacement are done.
- Everyone helps test and validate the final CVRP solution.
- Everyone must understand the whole toolbox, since the discussion covers every member.

---

## 2. Status board

Update your own row when your status changes.

| Component | Owner | Status | Notes |
|---|---|---|---|
| `core/Random.h` | Member 1 | Done | Seeded, non-copyable, helper functions |
| `core/GAConfig.h` | Member 1 | Done | Defaults + `validate()` |
| `core/Types.h` | Member 1 | Done | Fitness and feasibility function aliases |
| `chromosomes/` (base + 3 types) | Member 1 | Done | Run `test_chromosomes` to verify |
| `engine/GeneticAlgorithm.h` | TBD (Member 1 or 2) | Not started | Agree on the owner |
| Selection (2 methods) | Member 2 | Not started | |
| Replacement (2 methods) | Member 2 | Not started | |
| Multi-point crossover | Member 3 | Not started | |
| Order crossover (OX) | Member 3 | Not started | |
| Mutation (3 methods) | Member 4 | Not started | |
| Constraint handling | Member 4 | Not started | |
| CVRP model, decoder, fitness | Member 5 | Not started | |
| `examples/onemax_example.cpp` | TBD | Not started | Proves reusability |
| Final integration and testing | Everyone | Not started | |

Status values: Not started, In progress, In review, Done.

---

## 3. Project layout

```
Genetic-Algorithm-Toolbox/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── main.cpp
├── include/ga/
│   ├── ga.h
│   ├── core/
│   │   ├── Random.h
│   │   ├── GAConfig.h
│   │   └── Types.h
│   ├── chromosomes/
│   │   ├── Chromosome.h
│   │   ├── BinaryChromosome.h
│   │   ├── IntegerChromosome.h
│   │   └── FloatChromosome.h
│   ├── selection/
│   │   ├── Selection.h
│   │   └── (2 concrete selection files)
│   ├── replacement/
│   │   ├── Replacement.h
│   │   └── (2 concrete replacement files)
│   ├── crossover/
│   │   ├── Crossover.h
│   │   ├── MultiPointCrossover.h
│   │   └── OrderCrossover.h
│   ├── mutation/
│   │   ├── Mutation.h
│   │   └── (3 concrete mutation files)
│   ├── constraints/
│   │   ├── ConstraintHandler.h
│   │   └── (repair and/or penalty files)
│   └── engine/
│       └── GeneticAlgorithm.h
├── cvrp/
│   ├── CVRPInstance.h
│   ├── InstanceParser.h
│   ├── Decoder.h
│   ├── CVRPProblem.h
│   └── OutputFormatter.h
├── data/
├── examples/
│   └── onemax_example.cpp
└── tests/
    └── test_chromosomes.cpp
```

---

## 4. What every file does

### Root

| File | Purpose |
|---|---|
| `CMakeLists.txt` | Builds the CVRP program, the example, and each test. One executable per program. |
| `README.md` | This file. |
| `.gitignore` | Keeps build folders (e.g. `cmake-build-debug/`) out of the repo. |
| `main.cpp` | CVRP entry point: load instance, configure GA, run, print output. Keep it thin; it is only wiring. |

### `include/ga/ga.h`

Umbrella header. Users write one `#include` to get the whole toolbox. Every new header must be added here.

### `include/ga/core/` (Member 1): shared foundations

| File | Purpose |
|---|---|
| `Random.h` | One seeded RNG wrapper (`ga::Random`) used by every operator. Helpers: random integer in a range, random real in a range, a probability check, shuffle, and access to the raw engine. Non-copyable. |
| `GAConfig.h` | `ga::GAConfig`: all hyperparameters with default values (population size, generations, crossover rate, mutation rate, seed, elitism count, minimize flag). Includes `validate()`, which returns an empty string if the config is valid and an error message otherwise. |
| `Types.h` | `ga::FitnessFunction<C>` and `ga::FeasibilityFunction<C>`: the signatures a problem plugs into. |

### `include/ga/chromosomes/` (Member 1): the three representations

| File | Purpose |
|---|---|
| `Chromosome.h` | `ga::Chromosome<T>`: common base holding genes, fitness value, evaluated flag, feasible flag. |
| `BinaryChromosome.h` | Genes are 0 or 1. Configured by `BinarySpec` (length). |
| `IntegerChromosome.h` | Integer genes with bounds. Configured by `IntegerSpec` (length, lower, upper, **permutation** flag). Permutation mode means every value in the range appears exactly once. Required by Order Crossover and by the CVRP giant tour. |
| `FloatChromosome.h` | Floating-point genes with bounds. Configured by `FloatSpec` (length, lower, upper). |

Each type can create itself randomly from a spec and the shared `Random`, and has `is_valid(spec)`.

### Operator folders: one interface header each, then one file per concrete class

The interface header is written by Member 1. The owner of the folder adds the concrete classes, **one class per file**, so nobody edits the same file at the same time.

| Folder | Interface header | Concrete classes | Owner |
|---|---|---|---|
| `selection/` | `Selection.h` | 2 selection methods | Member 2 |
| `replacement/` | `Replacement.h` | 2 replacement methods | Member 2 |
| `crossover/` | `Crossover.h` | `MultiPointCrossover.h`, `OrderCrossover.h` | Member 3 |
| `mutation/` | `Mutation.h` | 3 mutation methods | Member 4 |

Notes:
- Multi-point crossover works on binary, integer, and float chromosomes.
- Order crossover works **only** on permutations and must reject anything else.

### `include/ga/constraints/` (Member 4, interface by Member 1)

| File | Purpose |
|---|---|
| `ConstraintHandler.h` | Interface for dealing with infeasible solutions. The engine applies it generically and it uses the user's feasibility function. |
| concrete files | The chosen strategy (repair, penalty, or both). Whatever is chosen must be justified in the report. |

It is separate from the operators so the engine can apply it without knowing the problem.

### `include/ga/engine/`

| File | Purpose |
|---|---|
| `GeneticAlgorithm.h` | The GA loop: initialize, evaluate, select, crossover, mutate, handle infeasible solutions, replace, repeat. Takes a config, the operators, and the two user functions. Knows nothing about CVRP. |

### `cvrp/` (Member 5): the only place CVRP knowledge lives

| File | Purpose |
|---|---|
| `CVRPInstance.h` | Instance data: customer coordinates, demands, vehicle capacity, number of vehicles, distance matrix. |
| `InstanceParser.h` | Reads the instance file format. |
| `Decoder.h` | Turns a chromosome (a permutation of customers) into vehicle routes. This is the representation and decoding part of the grade. |
| `CVRPProblem.h` | Provides the two functions given to the toolbox: fitness (total route distance) and feasibility (no vehicle over capacity). |
| `OutputFormatter.h` | Prints the solution in the required output format. |

### Other folders

| Folder | Purpose |
|---|---|
| `data/` | CVRP instance files for testing. |
| `examples/onemax_example.cpp` | A tiny non-CVRP problem using a binary chromosome. Proves the toolbox is reusable without rewriting the GA. |
| `tests/` | One test file per component (see section 10). |

---

## 5. How the pieces connect

```
        user problem (CVRP, OneMax, ...)
                 │  supplies: fitness function + feasibility function
                 ▼
┌───────────────────────────────────────────────┐
│               GeneticAlgorithm (engine)        │
│  uses: GAConfig, Random                        │
│  uses: Chromosome (Binary / Integer / Float)   │
│  uses: Selection, Crossover, Mutation,         │
│        Replacement, ConstraintHandler          │
└───────────────────────────────────────────────┘
                 │
                 ▼
          best solution found
```

One generation, in order:

1. Evaluate any chromosome that is not yet evaluated (fitness and feasibility).
2. Selection picks parents.
3. For each pair, the engine decides (using the crossover rate) whether to cross them.
4. For each child, the engine decides (using the mutation rate) whether to mutate it.
5. Infeasible children are handled by the constraint handler.
6. New children are evaluated.
7. Replacement builds the next population (the engine keeps the elite individuals).
8. Repeat until the generation limit.

The CVRP code only talks to the toolbox through the fitness and feasibility functions plus its decoder. If CVRP code ever appears inside `include/ga/`, that is a design bug.

---

## 6. Decisions we have made (rules)

These are fixed. If you disagree, raise it with the team before changing code.

1. **Namespace:** everything in the toolbox lives in `namespace ga`. Write `ga::Random`, `ga::BinaryChromosome`, and so on.
2. **Higher fitness is always better inside the toolbox.** If `GAConfig::minimize` is true, the engine converts the user's value once, right after evaluation. Selection and replacement never think about direction.
3. **Roulette-style selection needs non-negative values.** Member 2 must handle this (for example by shifting fitness values so the minimum is not negative).
4. **Rates are decided by the engine, not by the operator.**
    - The crossover rate is the probability that a pair of parents is crossed. A crossover operator always crosses when it is called.
    - The mutation rate is **per chromosome**: the probability that a child is mutated. A mutation operator always mutates when it is called. Any per-gene probability is a parameter of that specific mutation class.
5. **Operator-specific settings go in the operator's constructor** (tournament size, number of crossover points, and so on), not in `GAConfig`.
6. **One shared RNG.** Every operator receives `ga::Random&` and uses its helpers. No `rand()`, no private generators, no distributions created inside operators.
7. **`Random` is never copied.** Always pass it by reference (`ga::Random&`). Copying it is a compile error on purpose.
8. **Reproducibility:** `GAConfig` has a fixed default seed. The same seed must give the same result, so we can reproduce each other's bugs. Use `ga::Random rng;` (no argument) only when you want a random seed.
9. **Standard library only.** No GA libraries. Ask the TA before using any third-party library.

---

## 7. Chromosome rules for operator authors

Read this before writing any operator.

1. **Reading genes:** use `genes()`. It never affects the evaluated flag.
2. **Changing genes:** use `mutable_genes()`. It automatically marks the chromosome as not evaluated, so a stale fitness can never be reused.
3. **Never keep a reference** returned by `mutable_genes()` across other modifications.
4. **Creating offspring:** build a new chromosome from a gene vector. It starts unevaluated automatically.
5. **Only the engine** calls `set_fitness()` and `set_feasible()`. Operators must not.
6. **Chromosomes hold data and validity checks only.** No crossover, mutation, or fitness logic inside them.
7. **Specs describe the representation** (length, bounds, permutation flag). The population shares one spec instead of every chromosome carrying its own bounds. Operators that need bounds (float or integer mutation) receive the spec.
8. **Copying is safe.** A copied chromosome keeps its fitness. Changing the copy through `mutable_genes()` resets only the copy.
9. **Binary genes are `uint8_t`.** When printing, cast to `int` first, or you will see garbage characters.

| Type | Gene type | Spec | Valid when |
|---|---|---|---|
| `ga::BinaryChromosome` | `uint8_t` | `BinarySpec` | length matches, every gene is 0 or 1 |
| `ga::IntegerChromosome` | `int` | `IntegerSpec` | length matches, in bounds, and in permutation mode each value appears exactly once |
| `ga::FloatChromosome` | `double` | `FloatSpec` | length matches, every gene within bounds |

---

## 8. Operator contracts

These describe what each interface should provide. Member 1 turns them into the interface headers; once those are pushed, the headers are the final authority and this section must be updated to match.

| Interface | Receives | Produces | Notes |
|---|---|---|---|
| `Selection` | the current population (read-only), how many individuals to pick, `Random&` | the chosen parents | Does not modify the population. Assumes higher fitness is better. |
| `Crossover` | two parents (read-only), `Random&` | two children | Always crosses when called. Children start unevaluated. **OX must reject non-permutations.** |
| `Mutation` | one chromosome (modified in place), `Random&` | nothing (changes the chromosome) | Always mutates when called. Must go through `mutable_genes()`. Must respect bounds, and must keep permutations valid when in permutation mode. |
| `Replacement` | the parent population, the offspring, `Random&` | the next population | Same population size as before. The engine handles elitism separately. |
| `ConstraintHandler` | a chromosome and the user's feasibility function | a feasible chromosome, or a penalised fitness (depends on the chosen strategy) | Member 4 decides the strategy. Must not contain any CVRP logic. |

General rules for every operator:
- It only sees chromosomes, specs, and the RNG. It never sees the problem.
- Each concrete operator is its own class in its own file.
- Each operator documents in a comment which chromosome types it supports.

---

## 9. Build and run

We use CMake (CLion works out of the box).

**Rule: every program has exactly one `main`, and its own `add_executable` in `CMakeLists.txt`.** If two files with a `main` end up in the same executable, the build fails with `multiple definition of main`. `main()` must also be global, never inside a namespace.

| Executable | Source | Purpose |
|---|---|---|
| `Genetic_Algorithm_Toolbox` | `main.cpp` | The CVRP program |
| `onemax_example` | `examples/onemax_example.cpp` | Non-CVRP example |
| `test_chromosomes` | `tests/test_chromosomes.cpp` | Chromosome and core tests |

Command line:

```
cmake -S . -B build
cmake --build build
./build/test_chromosomes
```

In CLion: reload CMake (Tools > CMake > Reload CMake Project) after editing `CMakeLists.txt`, then choose the executable from the run dropdown.

Adding a new test or program: add one line `add_executable(<name> <path-to-file>)` to `CMakeLists.txt`.

Requirements: a C++17 compiler (set in CMake).

---

## 10. Testing

- `tests/` has one test file per component, each with its own `main` and its own executable.
- We use a small `CHECK` macro instead of `assert`, because `assert` is removed in Release builds. A test prints the failing condition and line number, and returns a non-zero exit code on failure.
- **Member 1** tests: the same seed gives the same sequence, config validation, every chromosome initializes validly, permutation mode always produces valid permutations, the evaluated flag behaves correctly on copy and modification.
- **Each owner** tests their own operators. Examples: crossover of two valid permutations yields a valid permutation, mutation keeps genes in bounds, selection returns the requested number of individuals.
- **`examples/onemax_example.cpp`** is the end-to-end check for the toolbox on its own.
- **Everyone** validates the final CVRP output together.

A component is "Done" on the status board only when its test passes.

---

## 11. Git workflow

1. Never push directly to `main`.
2. One branch per member, named after the area (for example `member2-selection`).
3. Small, frequent commits with clear messages.
4. Merge to `main` through a pull request. Ask for one other member to look at it.
5. The interface headers belong to Member 1. If you need an interface changed, ask Member 1 instead of editing it.
6. Do not edit someone else's folder. Ask the owner.
7. Never commit build folders (`cmake-build-*`, `build/`). They belong in `.gitignore`.
8. Pull from `main` before you start working each day.

---

## 12. Code conventions

1. Header guards in every header (`#ifndef ... #define ... #endif`).
2. **No `using namespace` in headers.** It leaks into every file that includes it. Write `std::vector`, not `vector`.
3. Include what you use (`<vector>`, `<random>`, `<functional>`), do not rely on another header to include it for you.
4. Everything goes inside `namespace ga`.
5. One class per file, file named after the class.
6. Pass large objects by `const&` (read-only) or `&` (modify). Do not pass chromosomes or populations by value.
7. Functions defined in headers must be `inline` (or be templates or members of a class), otherwise two `.cpp` files including the header cause a linker error.
8. Give every variable an initial value. Reading an uninitialized variable is undefined behavior.
9. Naming: `PascalCase` for classes and structs, `snake_case` for functions and variables, matching the code already in the repo.
10. Short comments that say **why**, not what.

---

## 13. Adding a new operator

1. Find the folder for its type (for example `mutation/`).
2. Read the interface header in that folder.
3. Create a new header with one class that implements the interface. Document which chromosome types it supports.
4. Follow the chromosome rules in section 7 and the contract in section 8.
5. Add it to `ga.h`.
6. Add a test file in `tests/` and a matching `add_executable` line.
7. Update your row on the status board.

---

## 14. Order of work and milestones

1. **Member 1:** core (RNG, config, types), the chromosome base and types (done), then **all interface headers**. Push these first, even before they are polished, because everyone else is blocked on them.
2. **Members 2, 3, 4:** implement operators against the pushed interfaces.
3. **Member 1 (or Member 2):** the engine loop.
4. **Member 5:** the CVRP model, decoder, fitness, feasibility, then integration.
5. **Everyone:** testing, tuning, and preparing for the discussion.

| Milestone | Target date |
|---|---|
| Interface headers pushed | TBD |
| All operators implemented | TBD |
| Engine working on `onemax_example` | TBD |
| CVRP running end to end | TBD |
| Testing and tuning finished | TBD |
| Discussion preparation | TBD |
| **Submission deadline** | Check Google Classroom |

---

## 15. Still undecided

Settle these as a team and move them to section 6.

- [ ] **Constraint handling strategy:** repair, penalty, or decoder-based. Member 4 proposes, the team agrees.
- [ ] **Which chromosome CVRP uses.** The natural choice is an integer permutation (a giant tour of customers). The toolbox must still support all three.
- [ ] **Owner of the engine** (`GeneticAlgorithm.h`): Member 1 or Member 2.
- [ ] **Stopping conditions beyond the generation limit** (for example stop after N generations without improvement).
- [ ] **Which selection, replacement, and mutation methods exactly.** Member 2 and Member 4 choose from what was covered in the lectures and labs.

---

## 16. Design justification (for the discussion)

Part of the grade is explaining design choices. Every member should be able to answer these.

- **Why a templated parent with three children?** The parent is templated on the gene type, so operators like multi-point crossover work on any gene type without being written three times. The three child classes still exist, so each representation has its own initialization and validity rules.
- **Why specs instead of storing bounds in each chromosome?** A population has many chromosomes that all share the same length and bounds. Storing them once saves memory and keeps copying cheap.
- **Why is permutation a flag and not a fourth class?** The assignment asks for three representations. A flag on the integer chromosome keeps it at three, and Order Crossover checks its input at runtime. A separate class would let the compiler prevent misuse, which is a valid alternative worth mentioning.
- **Why does `mutable_genes()` reset the evaluated flag?** It makes a stale fitness impossible: any code that changes genes automatically invalidates the cached fitness, so nobody can forget to do it.
- **Why one shared RNG?** Reproducibility (same seed, same result) and one place to look when debugging.
- **Why "higher is better" inside the toolbox?** Selection methods assume it. Converting once in the engine means no operator needs to know whether the problem minimizes or maximizes.
- **Why does the engine decide the crossover and mutation rates?** Operators stay simple (they always do their job when called), and the probabilities are applied in one place.
- **Why `std::function` for the fitness and feasibility functions?** A user can pass a lambda, a free function, or a method, with no inheritance needed. That is the easiest way to reuse the toolbox on a new problem.
- **Why a separate constraint handler?** So the engine can handle infeasible solutions generically, using only the user's feasibility function, with no problem-specific code in the toolbox.

---

## 17. CVRP representation FAQ

**Why does the toolbox have three representations if CVRP uses one or two?**
The assignment asks for a general toolbox, so it must support all three. CVRP is the test problem, and it naturally fits a permutation, not every representation.

**What does CVRP use?**
The natural choice is an integer chromosome in permutation mode, holding every customer exactly once (a "giant tour"). The decoder then splits it into vehicle routes whenever the next customer would exceed the vehicle capacity. This makes every solution feasible by construction, and Order Crossover is meaningful on it.

**How could float or binary represent CVRP if asked?**
- **Float:** random keys. Each customer gets a real number, and sorting the numbers gives the visiting order.
- **Binary:** the bits can be decoded into integers, which are then turned into an order. It is possible but awkward, which is a good reason CVRP does not use it.

**Where does the decoder live?**
In `cvrp/Decoder.h`, never in the toolbox.

---

## 18. Glossary

| Term | Meaning |
|---|---|
| Chromosome | One candidate solution: a sequence of genes plus its fitness. |
| Gene | One element of a chromosome (a bit, an integer, or a real number). |
| Population | The set of chromosomes in one generation. |
| Spec | A small description of a representation (length, bounds, permutation flag), shared by the whole population. |
| Fitness | A number saying how good a chromosome is. Higher is better inside the toolbox. |
| Evaluated | The chromosome's fitness is up to date. Resets when its genes change. |
| Feasible | The chromosome satisfies the problem's constraints (for CVRP, no vehicle over capacity). |
| Elitism | Copying the best individuals unchanged into the next generation. |
| Permutation | An integer chromosome where each value appears exactly once. |
| Decoder | Code that converts a chromosome into a real solution (for CVRP, vehicle routes). |
| Generation | One full round of selection, crossover, mutation, and replacement. |

---

## 19. Grading checklist

| Area | Marks | Where it lives |
|---|---|---|
| Toolbox design and structure | 6 | whole `include/ga/` |
| Chromosome representations | 6 | `chromosomes/` |
| GA configuration and implementation | 8 | `core/GAConfig.h`, `engine/` |
| Selection | 10 | `selection/` |
| Crossover | 10 | `crossover/` |
| Mutation | 12 | `mutation/` |
| Replacement | 8 | `replacement/` |
| Problem modeling (representation, decoding) | 10 | `cvrp/Decoder.h`, `cvrp/CVRPInstance.h` |
| Constraint handling (feasibility check) | 10 | `constraints/`, `cvrp/CVRPProblem.h` |
| Fitness evaluation function | 10 | `cvrp/CVRPProblem.h` |
| Correct usage of the toolbox | 6 | `main.cpp` |
| Performance and solution quality | 4 | tuning, `data/` |

Part of the grade is each member's understanding and ability to justify design choices in the discussion. Every member should be able to explain the whole design, not only their own folder.

---

## 20. Submission

One zip on Google Classroom, named:

```
TAName_Group_AssignmentNumber_ID1_ID2_ID3_ID4_ID5
```

Check the deadline on Google Classroom. Late submissions are not accepted.