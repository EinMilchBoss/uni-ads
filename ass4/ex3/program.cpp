#include <iostream>
#include <chrono>
#include <thread>

#include "rng.hpp"

using namespace std::literals::chrono_literals;

void swap(int32_t &a, int32_t &b)
{
    int32_t tmp = a;
    a = b;
    b = tmp;
}

int32_t prepare_partitions(int32_t *xs, int32_t xs_len)
{

    // Just use the first element as our pivot element.
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

void quick_sort(int32_t *xs, int32_t xs_len)
{
    if (xs_len <= 1)
        return;

    int32_t pivot_idx = prepare_partitions(xs, xs_len);

    int32_t skip_len = pivot_idx + 1;
    quick_sort(xs, pivot_idx);
    quick_sort(xs + skip_len, xs_len - skip_len);
}

void insertion_sort(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len; i++)
    {
        const int32_t x = xs[i];

        int32_t j = i - 1;
        while (j >= 0 && xs[j] > x)
        {
            xs[j + 1] = xs[j];
            j -= 1;
        }

        xs[j + 1] = x;
    }
}

void bubble_sort(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len - 1; i++)
    {
        for (int32_t j = xs_len - 1; j > i; j--)
        {
            if (xs[j - 1] > xs[j])
                swap(xs[j - 1], xs[j]);
        }
    }
}

void selection_sort(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len - 1; i++)
    {
        int32_t smallest_idx = i;

        for (int32_t j = i + 1; j < xs_len; j++)
        {
            const int32_t x = xs[smallest_idx];
            if (xs[j] < x)
                smallest_idx = j;
        }

        swap(xs[i], xs[smallest_idx]);
    }
}

void init_random_array(int32_t *xs, int32_t xs_len, rng &rng)
{
    // Reset array to ascending numbers starting from 0.
    for (int32_t i = 0; i < xs_len; i++)
        xs[i] = i;

    // Shuffle array randomly.
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

/**
 * `elapsed_time_already_printed` avoids repetitive print statements in case of high frequency.
 */
void print_elapsed_time_info(std::chrono::seconds elapsed_time, bool &elapsed_time_already_printed)
{
    const long long seconds = elapsed_time.count();
    if (seconds % 5 == 0)
    {
        if (!elapsed_time_already_printed)
        {
            std::cout << seconds << "s are elapsed...\n";
            elapsed_time_already_printed = true;
        }
    }
    else
    {
        elapsed_time_already_printed = false;
    }
}

void perform_sort(int32_t *xs, int32_t xs_len, rng &rng, void (*sort)(int32_t *, int32_t))
{
    init_random_array(xs, xs_len, rng);
    sort(xs, xs_len);

    if (!is_sorted(xs, xs_len))
        throw std::runtime_error("sorting did not work");
}

void print_array(int32_t *xs, int32_t xs_len)
{
    for (int32_t i = 0; i < xs_len; i++)
        std::cout << xs[i] << ' ';
    std::cout << '\n';
}

void run_test(
    const char *algo_name,
    const std::chrono::seconds test_duration,
    const int32_t n,
    const unsigned int seed,
    void (*sort)(int32_t *, int32_t))
{
    std::cout << "Start run for " << algo_name << ".\n";

    std::unique_ptr<int32_t[]> xs_handle = std::make_unique<int32_t[]>(static_cast<size_t>(n));
    int32_t *xs = xs_handle.get();

    rng rng(0, n - 1, seed);

    std::chrono::seconds elapsed_time;
    bool elapsed_time_already_printed = false;
    size_t iterations = 0;

    const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    do
    {
        perform_sort(xs, n, rng, sort);
        iterations += 1;

        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

        print_elapsed_time_info(elapsed_time, elapsed_time_already_printed);
    } while (elapsed_time < test_duration);

    std::cout << "Amount of iterations for " << algo_name << ": " << iterations << ".\n";
}

int main()
{
    constexpr std::chrono::seconds test_duration = 10s;
    constexpr int32_t n = 100;

    std::random_device seed_generator;
    const unsigned int seed = seed_generator();
    std::cout << "Using seed: " << seed << ".\n";

    run_test("quick sort", test_duration, n, seed, quick_sort);
    run_test("insertion sort", test_duration, n, seed, insertion_sort);
    run_test("bubble sort", test_duration, n, seed, bubble_sort);
    run_test("selection sort", test_duration, n, seed, selection_sort);

    return 0;
}