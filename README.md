# Genetic Algorithm Toolbox + CVRP

Soft Computing, Assignment 1 (Cairo University, FCAI).

A reusable genetic algorithm (GA) toolbox written from scratch in C++, plus a program that uses it to solve the Capacitated Vehicle Routing Problem (CVRP).

**Core rule:** the toolbox (`include/ga/`) must know nothing about CVRP. All CVRP knowledge lives in `cvrp/`. The only things CVRP gives the toolbox are a fitness function and a feasibility check function.

---

## 1. Team ownership

| Member | Area | Where you work |
|---|---|---|
| **Member 1** | Toolbox structure, interfaces, config, RNG, 3 chromosome representations | `include/ga/core/`, `include/ga/chromosomes/`, all interface headers, `ga.h`, CMake, README |
| **Member 2** | 2 selection methods, 2 replacement methods | `include/ga/selection/`, `include/ga/replacement/` |
| **Member 3** | Multi-point crossover, Order crossover (OX) | `include/ga/crossover/` |
| **Member 4** | 3 mutation methods, constraint handling for infeasible solutions | `include/ga/mutation/`, `include/ga/constraints/` |
| **Member 5** | CVRP modeling, decoder, fitness, integration and final testing | `cvrp/`, `main.cpp`, `data/` |

Collaboration:
- Member 2 helps Member 5 with testing and integration once selection and replacement are done.
- Everyone helps test and validate the final CVRP solution.
- Everyone must understand the whole toolbox, since the discussion covers every member.

---

## 2. Project layout

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
```

---

## 3. What every file does

### Root

| File | Purpose |
|---|---|
| `CMakeLists.txt` | Builds the CVRP program, the example, and the tests. |
| `README.md` | This file: layout, ownership, conventions, how to add an operator. |
| `.gitignore` | Keeps build folders (e.g. `cmake-build-debug/`) out of the repo. |
| `main.cpp` | CVRP entry point: load instance, configure GA, run, print output. Keep it thin; it is only wiring. |

### `include/ga/ga.h`

Umbrella header. Users write one `#include` to get the whole toolbox.

### `include/ga/core/` (Member 1): shared foundations

| File | Purpose |
|---|---|
| `Random.h` | One seeded RNG wrapper used by every operator. Makes runs reproducible. Nobody creates their own RNG. |
| `GAConfig.h` | All hyperparameters with default values (population size, generations, crossover rate, mutation rate, elitism count, seed). The user can override any of them. |
| `Types.h` | Shared type aliases, mainly the fitness function and feasibility function signatures that a problem plugs into. |

### `include/ga/chromosomes/` (Member 1): the three representations

| File | Purpose |
|---|---|
| `Chromosome.h` | Common base: genes, fitness value, feasible flag, evaluated flag. |
| `BinaryChromosome.h` | Binary representation. Random initialization of a given length. |
| `IntegerChromosome.h` | Integer representation with bounds. Has a **permutation mode** (each value appears exactly once), required by Order Crossover and by the CVRP giant-tour encoding. |
| `FloatChromosome.h` | Floating-point representation with bounds. |

Each representation also provides a validity check (within bounds, valid permutation if in permutation mode) and a way to print itself for debugging.

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
| `tests/` | One test per component (see section 6). |

---

## 4. How the pieces connect

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

The CVRP code only talks to the toolbox through the fitness and feasibility functions plus its decoder. If CVRP code ever appears inside `include/ga/`, that is a design bug.

---

## 5. Team conventions

1. **One class per file.** Interface in the folder's base header, each concrete operator in its own header.
2. **One RNG.** Every operator receives the shared RNG. No `rand()`, no private generators.
3. **Operators never touch the problem.** They only see chromosomes and the RNG.
4. **Minimize vs. maximize:** decide once as a team and write it here. (Open decision, see below.)
5. **No GA libraries.** Standard library only. Confirm with the TA before using any third-party library (e.g. for numerics).
6. **Do not edit someone else's folder.** Ask the owner, or open a pull request.
7. **Never commit build folders.**

### Adding a new operator

1. Find the folder for its type (e.g. `mutation/`).
2. Read the interface header in that folder.
3. Create a new header with one class that implements the interface.
4. Add it to `ga.h`.
5. Add a test in `tests/`.

---

## 6. Testing

- `tests/` has one test file per component.
- Member 1 tests that every chromosome initializes within its bounds and that permutation mode always produces valid permutations.
- Each owner tests their own operators (for example: crossover of two valid permutations yields a valid permutation).
- `examples/onemax_example.cpp` is the end-to-end check for the toolbox on its own.
- Everyone validates the final CVRP output together.

---

## 7. Order of work

1. **Member 1:** core (RNG, config, types), the chromosome base, and **all interface headers**. Push these first, even before they are polished, because everyone else is blocked on them.
2. **Members 2, 3, 4:** implement operators against the pushed interfaces.
3. **Member 1 (or Member 2):** the engine loop.
4. **Member 5:** the CVRP model, decoder, fitness, feasibility, then integration.
5. **Everyone:** testing, tuning, and preparing for the discussion.

---

## 8. Open decisions (settle before writing code)

- [ ] **Templated vs. non-templated chromosome.** Templated means operators are header-only and `.cpp` files are mostly empty. Non-templated means a base class with subclasses and real `.cpp` files. Everyone's file layout follows from this choice.
- [ ] **Minimize or maximize.** Either the engine always maximizes, or the config has an explicit flag.
- [ ] **Constraint handling strategy:** repair, penalty, or decoder-based. Member 4 proposes, the team agrees.
- [ ] **Which chromosome the CVRP uses.** The natural choice is an integer permutation. The toolbox must still support all three, so be ready to explain in the discussion why CVRP uses one or two.
- [ ] **Operator signatures:** exactly what each operator receives and returns, and who owns randomness.

---

## 9. Grading checklist

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

## 10. Submission

One zip on Google Classroom, named:

```
TAName_Group_AssignmentNumber_ID1_ID2_ID3_ID4_ID5
```

Check the deadline on Google Classroom. Late submissions are not accepted.
