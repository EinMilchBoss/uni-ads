#include "matrix.cpp"

using namespace std::chrono_literals;

void test_sum(std::chrono::seconds time, std::chrono::milliseconds tolerance)
{
    std::chrono::milliseconds lower_bound = time - tolerance;
    std::chrono::milliseconds upper_bound = time + tolerance;

    size_t min_size = 1;
    size_t current_size = 1;

    while (true)
    {
        std::println("Testing for size {}...", current_size);

        matrix left(current_size, current_size);
        matrix right(current_size, current_size);

        auto [sum, calctime, iterations] = left.add(right);
        std::println("Took {} ({} iterations)", calctime, iterations);

        if (calctime < lower_bound)
        {
            min_size = current_size;
            current_size *= 2;
        }
        else if (calctime > upper_bound)
        {
            const size_t average_size = (min_size + current_size) / 2;
            current_size = average_size;
        }
        else
        {
            std::println("Final size reached for {} (with ±{}): {}", time, tolerance, current_size);
            break;
        }
    }
}

void test_mult(std::chrono::seconds time, std::chrono::milliseconds tolerance)
{
    std::chrono::milliseconds lower_bound = time - tolerance;
    std::chrono::milliseconds upper_bound = time + tolerance;

    size_t min_size = 1;
    size_t current_size = 1;

    while (true)
    {
        std::println("Testing for size {}...", current_size);

        matrix left(current_size, current_size);
        matrix right(current_size, current_size);

        auto [sum, calctime, iterations] = left.mult(right);
        std::println("Took {} ({} iterations)", calctime, iterations);

        if (calctime < lower_bound)
        {
            min_size = current_size;
            current_size *= 2;
        }
        else if (calctime > upper_bound)
        {
            const size_t average_size = (min_size + current_size) / 2;
            current_size = average_size;
        }
        else
        {
            std::println("Final size reached for {} (with ±{}): {}", time, tolerance, current_size);
            break;
        }
    }
}

int main()
{
    test_sum(1s, 200ms);
    test_mult(1s, 200ms);

    return 0;
}