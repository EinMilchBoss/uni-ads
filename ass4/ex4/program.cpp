#include <iostream>
#include <algorithm>

#include "rng.hpp"

void swap(int32_t &a, int32_t &b)
{
    int32_t tmp = a;
    a = b;
    b = tmp;
}

int32_t prepare_partitions(int32_t *a, int32_t a_len)
{
    int32_t pivot = a[0];

    int32_t pivot_idx = -1;

    for (int32_t i = 0; i < a_len; i++)
    {
        if (a[i] <= pivot)
        {
            pivot_idx += 1;
            swap(a[i], a[pivot_idx]);
        }
    }

    swap(a[0], a[pivot_idx]);

    return pivot_idx;
}

void quick_sort(int32_t *a, int32_t a_len)
{
    if (a_len <= 1)
        return;

    const int32_t pivot_idx = prepare_partitions(a, a_len);

    quick_sort(a, pivot_idx);
    quick_sort(a + pivot_idx + 1, a_len - pivot_idx - 1);
}

int32_t do_binary_search(int32_t *a, int32_t a_len, int32_t value, int32_t offset)
{
    if (a_len <= 0)
        return -1;

    const int32_t first_half = a_len / 2;
    const int32_t second_half = a_len - first_half - 1;
    const int32_t middle = a[first_half];

    if (value < middle)
        return do_binary_search(a, first_half, value, offset);
    else if (value > middle)
        return do_binary_search(a + first_half + 1, second_half, value, offset + first_half + 1);
    else
        return first_half + offset;
}

int32_t binary_search(int32_t *a, int32_t a_len, int32_t value)
{
    return do_binary_search(a, a_len, value, 0);
}

bool f(int32_t *a, int32_t a_len, int32_t sum)
{
    // nlog(n)
    quick_sort(a, a_len);

    // nlog(n)
    for (int32_t i = 0; i < a_len; i++)
    {
        const int32_t rest = sum - a[i];
        const int32_t pair_idx = binary_search(a, a_len, rest);
        if (pair_idx != -1 && pair_idx != i)
            return true;
    }

    return false;
}

bool naive_f(int32_t *a, int32_t a_len, int32_t sum)
{
    for (int32_t i = 0; i < a_len; i++)
    {
        const int32_t rest = sum - a[i];

        for (int32_t j = i + 1; j < a_len; j++)
        {
            if (a[j] == rest)
                return true;
        }
    }
    return false;
}

void init_random_array(int32_t *xs, int32_t xs_len, rng &rng)
{
    // Reset array to ascending numbers starting from 0.
    for (int32_t i = 0; i < xs_len; i++)
        xs[i] = rng.next();
}

void print_array(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len; i++)
        std::cout << xs[i] << ' ';
    std::cout << '\n';
}

int main()
{
    constexpr int32_t a_len = 10;
    int32_t a[a_len];

    std::random_device seed_generator;
    const unsigned int seed = seed_generator();
    std::cout << "Using seed: " << seed << ".\n";
    rng rng(0, a_len - 1, seed);

    for (int32_t i = 0; i < 1000000; i++)
    {
        init_random_array(a, a_len, rng);

        // Generate copy of our array to check if the sorting
        // algorithm is did something incorrectly.
        int32_t original[a_len];
        std::copy(std::begin(a), std::end(a), std::begin(original));

        // Test our naive implementation first, because
        // the actual implementation sorts the array.
        bool expected = naive_f(a, a_len, 10);
        bool actual = f(a, a_len, 10);

        if (expected != actual)
        {
            std::cerr << "Our implementation seems to deviate for:\n";
            print_array(original, a_len);
            print_array(a, a_len);
            std::cerr << "Expected: " << expected << '\n';
            std::cerr << "Actual  : " << actual << '\n';
            return 1;
        }
    }

    return 0;
}