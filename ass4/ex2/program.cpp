#include <iostream>
#include <cstdint>

#include "rng.hpp"

void one(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 1; i < xs_len; i++)
    {
        int32_t x = xs[i];

        // Search until the current index for a swap partner.
        int32_t swap_idx = 0;
        while (xs[swap_idx] < x && swap_idx <= i)
            swap_idx += 1;

        // Make room for the element.
        // We have to do it from right to left or else we would overwrite the values.
        for (int32_t j = i; j > swap_idx; j--)
            xs[j] = xs[j - 1];

        xs[swap_idx] = x;
    }
}

void two(int32_t *xs, int32_t xs_len)
{
    // Sorting only makes sense for at least 2 elements.
    for (int32_t i = xs_len - 1; i >= 1; i--)
    {
        // Go to i - 1 because we take pairs (i, i + 1) for each step.
        for (int32_t j = 0; j <= i - 1; j++)
        {
            // Check if the pair has to be swapped.
            // The biggest value has to be the rightmost one.
            if (xs[j] > xs[j + 1])
            {
                int32_t tmp = xs[j];
                xs[j] = xs[j + 1];
                xs[j + 1] = tmp;
            }
        }
    }
}

void three(int32_t *xs, int32_t xs_len)
{
    // We start with the second last element.
    // The last has no potential swap partners.
    for (int32_t i = xs_len - 2; i >= 0; i--)
    {
        // Save the current value before it's overwritten by swapping.
        int32_t x = xs[i];

        // Make room for a gap by swapping to the left.
        // In case it goes out of bounds, it is the biggest value so far.
        int32_t swap_idx = i + 1;
        while (xs[swap_idx] < x && swap_idx < xs_len)
        {
            xs[swap_idx - 1] = xs[swap_idx];
            swap_idx += 1;
        }

        // Place value into formed gap.
        xs[swap_idx - 1] = x;
    }
}

void swap(int32_t &a, int32_t &b)
{
    int32_t tmp = a;
    a = b;
    b = tmp;
}

int32_t prepare_partitions(int32_t *xs, int32_t xs_len, unsigned int seed)
{
    rng rng(0, xs_len - 1, seed);

    // Pull a random element to the front by swapping and
    // choose that first element as our pivot element.
    swap(xs[0], xs[rng.next()]);
    int32_t pivot = xs[0];

    // Start with -1 because we first increment and then swap values.
    // We do this, so we are not off by one at the end of swapping.
    int32_t pivot_idx = -1;

    for (int32_t i = 0; i < xs_len; i++)
    {
        if (xs[i] <= pivot)
        {
            pivot_idx += 1;
            swap(xs[pivot_idx], xs[i]);
        }
    }

    // The original pivot element always stays on index 0,
    // so we swap that with the pivot index to separate the
    // two partitions by the pivot element.
    swap(xs[0], xs[pivot_idx]);

    return pivot_idx;
}

void quicksort(int32_t *xs, int32_t xs_len, unsigned int seed)
{
    if (xs_len <= 1)
        return;

    int32_t pivot_idx = prepare_partitions(xs, xs_len, seed);

    int32_t skip_len = pivot_idx + 1;
    quicksort(xs, pivot_idx, seed);
    quicksort(xs + skip_len, xs_len - skip_len, seed);
}

void four(int32_t *xs, int32_t xs_len, unsigned int seed = 0)
{
    if (seed == 0)
    {
        std::random_device seed_generator;
        quicksort(xs, xs_len, seed_generator());
    }
    else
    {
        quicksort(xs, xs_len, seed);
    }
}

void init_random_array(int32_t *xs, int32_t xs_len, unsigned int seed)
{
    // Reset array to ascending numbers starting from 0.
    for (int32_t i = 0; i < xs_len; i++)
        xs[i] = i;

    // Shuffle array randomly.
    rng rng(0, static_cast<int32_t>(xs_len - 1), seed);
    for (int32_t i = 0; i < xs_len; i++)
    {
        int32_t swap_idx = rng.next();

        int32_t tmp = xs[i];
        xs[i] = xs[swap_idx];
        xs[swap_idx] = tmp;
    }
}

bool is_sorted(int32_t *xs, int32_t xs_len)
{
    int32_t predecessor = *xs;
    for (int32_t i = 1; i < xs_len; i++)
    {
        if (predecessor > xs[i])
            return false;
        predecessor = xs[i];
    }
    return true;
}

void print_array(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len; i++)
        std::cout << xs[i] << ' ';
    std::cout << '\n';
}

int main()
{
    std::random_device seed_generator;
    unsigned int seed = seed_generator();
    std::cout << "Using seed: " << seed << ".\n";

    constexpr int32_t xs_len = 100;
    int32_t xs[xs_len];

    for (int32_t i = 0; i < 100; i++)
    {
        std::cout << "Sorting...\n";
        init_random_array(xs, xs_len, seed);
        four(xs, xs_len, seed);

        if (is_sorted(xs, xs_len))
        {
            std::cout << "Sorted.\n";
        }
        else
        {
            std::cerr << "Did not sort.\n";
            return 1;
        }
    }

    return 0;
}