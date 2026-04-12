#include <iostream>
#include <stdexcept>
#include <random>
#include <memory>

#include "rng.hpp"

uint64_t insertion_sort(int32_t *xs, uint32_t xs_len)
{
    size_t steps = 0;

    for (int64_t i = 0; i < xs_len; i++)
    {
        const int32_t x = xs[i];

        int64_t j = i - 1;
        while (j >= 0 && xs[j] > x)
        {
            steps += 1;

            xs[j + 1] = xs[j];
            j -= 1;
        }

        xs[j + 1] = x;
    }

    return steps;
}

bool is_sorted(int32_t *xs, uint32_t xs_len)
{
    if (xs_len == 0)
        return true;

    int32_t last = *xs;
    for (uint32_t i = 1; i < xs_len; i++)
    {
        if (last > xs[i])
            return false;

        last = xs[i];
    }
    return true;
}

void shuffle_randomly(int32_t *xs, uint32_t xs_len, unsigned int seed)
{
    rng rng(0, static_cast<int32_t>(xs_len - 1), seed);
    for (uint32_t i = 0; i < xs_len; i++)
    {
        int32_t swap_idx = rng.next();

        int32_t tmp = xs[i];
        xs[i] = xs[swap_idx];
        xs[swap_idx] = tmp;
    }
}

void init_ordered_array(int32_t *xs, uint32_t xs_len)
{
    for (int32_t i = 0; i < static_cast<int32_t>(xs_len); i++)
        xs[i] = i + 1;
}

int main()
{
    std::random_device seed_generator;

    constexpr uint32_t xs_len = 100000;
    std::unique_ptr<int32_t[]> xs = std::make_unique<int32_t[]>(xs_len);

    constexpr uint16_t runs = 100;
    uint64_t step_sum = 0;
    for (uint16_t i = 0; i < runs; i++)
    {
        unsigned int seed = seed_generator();

        init_ordered_array(xs.get(), xs_len);
        shuffle_randomly(xs.get(), xs_len, seed);
        size_t steps = insertion_sort(xs.get(), xs_len);

        if (!is_sorted(xs.get(), xs_len))
        {
            std::cerr << "Sorting somehow did not work.\n";
            return 1;
        }

        step_sum += steps;
    }

    const uint64_t avg = step_sum / runs;
    std::cout << "Total stats:\n";
    std::cout << "- n: " << xs_len << ".\n";
    std::cout << "- Runs: " << runs << ".\n";
    std::cout << "- AVG: " << avg << ".\n";

    return 0;
}