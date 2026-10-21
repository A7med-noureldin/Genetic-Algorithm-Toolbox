cmake_minimum_required(VERSION 4.0.2)
project(Genetic_Algorithm_Toolbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(Genetic_Algorithm_Toolbox main.cpp
        include/ga/ga.h
        include/ga/core/Random.h
        include/ga/core/GAConfig.h
        include/ga/core/Types.h
        include/ga/chromosomes/Chromosome.h
        include/ga/chromosomes/BinaryChromosome.h
        include/ga/chromosomes/IntegerChromosome.h
        include/ga/chromosomes/FloatChromosome.h
        include/ga/selection/Selection.h
        include/ga/replacement/Replacement.h
        include/ga/crossover/Crossover.h
        include/ga/crossover/MultiPointCrossover.h
        include/ga/crossover/OrderCrossover.h
        include/ga/mutation/Mutation.h
        include/ga/constraints/ConstraintHandler.h
        include/ga/engine/GeneticAlgorithm.h
        cvrp/CVRPInstance.h
        cvrp/InstanceParser.h
        cvrp/Decoder.h
        cvrp/CVRPProblem.h
        cvrp/OutputFormatter.h
)
add_executable(onemax_example examples/onemax_example.cpp)
add_executable(test_chromosomes tests/test_chromosomes.cpp)