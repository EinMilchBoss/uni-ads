#include <iostream>
#include <array>
#include <cstdint>

template <size_t N>
size_t index(size_t i, size_t j)
{
    return i * N + j;
}

template <size_t N>
int32_t max_part_sum_1d(std::array<int32_t, N> values)
{
    int32_t global_max = INT32_MIN;
    int32_t local_max = 0;

    for (size_t i = 0; i < N; i++)
    {
        int32_t tmp = local_max + values[i];

        if (values[i] > tmp)
            local_max = values[i];
        else
            local_max = tmp;

        if (local_max > global_max)
            global_max = local_max;
    }

    return global_max;
}

template <size_t N>
int32_t max_part_sum_2d(std::array<int32_t, N * N> values)
{
    int32_t max = INT32_MIN;

    for (size_t i1 = 0; i1 < N; i1++)
    {
        std::array<int32_t, N> cols{};
        for (size_t i2 = i1; i2 < N; i2++)
        {
            // Don't recalculate whole array, just add values of new line.
            for (size_t j = 0; j < N; j++)
                cols[j] += values[index<N>(i2, j)];

            // Reduce it to the 1d problem.
            max = std::max(max, max_part_sum_1d(cols));
        }
    }

    return max;
}

int main()
{
    int32_t max_part_sum = max_part_sum_2d<3>(
        {1, 2, 3,
         4, 5, 6,
         7, 8, 9});
    std::cout << max_part_sum << std::endl;

    return 0;
}