#include <iostream>
#include <cstdint>

void merge(int32_t *merged, int32_t *lefts, int32_t lefts_len, int32_t *rights, int32_t rights_len)
{
    int32_t m = 0, l = 0, r = 0;
    while (l < lefts_len && r < rights_len)
    {
        if (lefts[l] < rights[r])
            merged[m++] = lefts[l++];
        else
            merged[m++] = rights[r++];
    }

    while (l < lefts_len)
        merged[m++] = lefts[l++];

    while (r < rights_len)
        merged[m++] = rights[r++];
}

int32_t *do_merge_sort(const int32_t *xs, int32_t xs_len, int32_t f, int32_t l)
{
    int32_t len = l - f + 1;
    int32_t *merged = new int32_t[static_cast<size_t>(len)];

    if (len == 1)
    {
        merged[0] = xs[f];
        return merged;
    }

    int32_t half = len / 2;
    int32_t lefts_len = half;
    int32_t rights_len = len - half;
    int32_t *lefts = do_merge_sort(xs, xs_len, f, f + half - 1);
    int32_t *rights = do_merge_sort(xs, xs_len, f + half, l);

    merge(merged, lefts, lefts_len, rights, rights_len);

    delete[] lefts;
    delete[] rights;

    return merged;
}

int32_t *merge_sort(const int32_t *xs, int32_t xs_len)
{
    if (xs_len <= 0)
        return nullptr;

    return do_merge_sort(xs, xs_len, 0, xs_len - 1);
}

int main()
{
    int32_t xs[] = {1, -1, 3, 2, -7, -8, 9, 0};
    int32_t *sorted = merge_sort(xs, 8);
    for (int32_t i = 0; i < 8; i++)
    {
        std::cout << sorted[i] << '\n';
    }

    return 0;
}