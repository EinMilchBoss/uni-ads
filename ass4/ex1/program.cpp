#include <iostream>
#include <cstdint>

void inner_loop(int32_t *xs, int32_t xs_len, int32_t j, int32_t x)
{
    if (j >= 0 && xs[j] > x)
    {
        xs[j + 1] = xs[j];
        inner_loop(xs, xs_len, j - 1, x);
    }
    else
    {
        xs[j + 1] = x;
    }
}

void outer_loop(int32_t *xs, int32_t xs_len, int32_t i)
{
    if (i >= xs_len)
        return;

    inner_loop(xs, xs_len, i - 1, xs[i]);
    outer_loop(xs, xs_len, i + 1);
}

void insertion_sort(int32_t *xs, int32_t xs_len)
{
    if (xs_len <= 1)
        return;

    outer_loop(xs, xs_len, 1);
}

int main()
{
    int32_t xs[] = {1, 5, 3, 7, 2, 7, 0, 2, -1};
    int32_t xs_len = sizeof(xs) / sizeof(*xs);
    insertion_sort(xs, xs_len);

    for (int32_t x : xs)
        std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}