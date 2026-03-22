#include <print>
#include <cstdint>
#include <iostream>

uint32_t euclidean_gcd_rec(uint32_t a, uint32_t b)
{
    uint32_t r = a % b;
    a = b;
    b = r;

    if (r == 0)
        return a;
    else
        return euclidean_gcd_rec(a, b);
}

uint32_t euclidean_gcd_iter(uint32_t a, uint32_t b)
{
    uint32_t r = 0;
    do
    {
        r = a % b;
        a = b;
        b = r;
    } while (r != 0);
    return a;
}

uint32_t lcm(uint32_t a, uint32_t b)
{
    uint32_t prod_a = a;
    uint32_t prod_b = b;

    while (prod_a != prod_b)
    {
        if (prod_a < prod_b)
            prod_a += a;
        else if (prod_a > prod_b)
            prod_b += b;
    }

    return prod_a;
}

void test_gcds()
{
    std::println("GCDs:");

    uint32_t inputs[][2] = {
        {12, 36},
        {36, 12},
        {9, 12},
        {24, 5},
    };
    uint32_t expecteds[] = {
        12,
        12,
        3,
        1,
    };

    for (int i = 0; i < 4; i++)
    {
        uint32_t actual_iter = euclidean_gcd_iter(inputs[i][0], inputs[i][1]);
        uint32_t actual_rec = euclidean_gcd_rec(inputs[i][0], inputs[i][1]);
        std::println("Iter: %d", actual_iter == expecteds[i]);
        std::println("Rec: %d", actual_rec == expecteds[i]);
    }
}

void test_lcm()
{
    std::println("LCM:");

    uint32_t inputs[][2] = {
        {12, 36},
        {36, 12},
        {3, 4},
        {3, 7},
    };
    uint32_t expecteds[] = {
        36,
        36,
        12,
        21,
    };

    for (int i = 0; i < 4; i++)
    {
        uint32_t actual = lcm(inputs[i][0], inputs[i][1]);
        std::println("Iter: {}", actual == expecteds[i]);
    }
}

int main()
{
    test_gcds();
    test_lcm();

    for (uint32_t a = 30; a <= 40; a++)
    {
        for (uint32_t b = 30; b <= 40; b++)
        {
            std::println("");
        }
    }

    return 0;
}