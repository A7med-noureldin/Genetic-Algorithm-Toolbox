//
// Created by hazem on 10/19/26.
//

#include <iostream>
#include <vector>
#include <stdexcept>
#include "../include/ga/ga.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::cout << "FAILED: " #cond \
    "  (line " << __LINE__ << ")\n"; ++failures; } } while (0)

int main()
{
    // Random: same seed gives the same sequence, different seed differs
    {
        ga::Random a(7), b(7), c(8);
        bool same = true, differs = false;
        for (int i = 0; i < 50; ++i)
        {
            int x = a.next_int(0, 1000), y = b.next_int(0, 1000), z = c.next_int(0, 1000);
            if (x != y) same = false;
            if (x != z) differs = true;
        }
        CHECK(same);
        CHECK(differs);
    }

    // GAConfig
    {
        ga::GAConfig cfg;
        CHECK(ga::validate(cfg).empty());
        cfg.population_size = 1;
        CHECK(!ga::validate(cfg).empty());
    }

    ga::Random rng(42);

    // Binary
    {
        ga::BinarySpec spec{20};
        bool allValid = true;
        for (int i = 0; i < 100; ++i)
            if (!ga::BinaryChromosome(spec, rng).is_valid(spec)) allValid = false;
        CHECK(allValid);
    }

    // Float
    {
        ga::FloatSpec spec{10, -5.0, 5.0};
        bool allValid = true;
        for (int i = 0; i < 100; ++i)
            if (!ga::FloatChromosome(spec, rng).is_valid(spec)) allValid = false;
        CHECK(allValid);
    }

    // Integer, normal mode
    {
        ga::IntegerSpec spec{10, 0, 9, false};
        bool allValid = true;
        for (int i = 0; i < 100; ++i)
            if (!ga::IntegerChromosome(spec, rng).is_valid(spec)) allValid = false;
        CHECK(allValid);
    }

    // Integer, permutation mode
    {
        ga::IntegerSpec spec{10, 1, 10, true};
        bool allValid = true;
        for (int i = 0; i < 100; ++i)
            if (!ga::IntegerChromosome(spec, rng).is_valid(spec)) allValid = false;
        CHECK(allValid);

        // a duplicate must be detected
        ga::IntegerChromosome bad(std::vector<int>{1, 1, 3, 4, 5, 6, 7, 8, 9, 10});
        CHECK(!bad.is_valid(spec));

        // range does not match length: must throw
        ga::IntegerSpec wrong{10, 1, 5, true};
        bool threw = false;
        try { ga::IntegerChromosome c(wrong, rng); }
        catch (const std::invalid_argument&) { threw = true; }
        CHECK(threw);
    }

    // evaluated flag behaviour
    {
        ga::BinaryChromosome original(std::vector<uint8_t>{1, 0, 1});
        original.set_fitness(2.0);
        CHECK(original.is_evaluated());

        ga::BinaryChromosome copy = original;
        CHECK(copy.is_evaluated());           // a copy keeps the fitness
        copy.mutable_genes()[0] = 0;
        CHECK(!copy.is_evaluated());          // changing genes resets it on the copy only
        CHECK(original.is_evaluated());       // original untouched

        original.genes();                     // plain reading must not reset it
        CHECK(original.is_evaluated());
    }

    std::cout << (failures == 0 ? "All tests passed\n" : "Some tests FAILED\n");
    return failures == 0 ? 0 : 1;
}