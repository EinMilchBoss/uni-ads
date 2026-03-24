#include <print>
#include <random>
#include <chrono>
#include <functional>

template <typename F>
std::pair<std::invoke_result_t<F>, std::chrono::milliseconds> measure_time_ms(F &&f)
{
    auto start = std::chrono::steady_clock::now();
    auto result = std::forward<F>(f)();
    auto end = std::chrono::steady_clock::now();

    return {
        result,
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start),
    };
}

class matrix
{
public:
    const size_t m, n;
    const size_t size;

private:
    std::vector<int32_t> elements_;

public:
    matrix(size_t m, size_t n) : m(m), n(n), size(m * n), elements_(size, 0)
    {
    }

    inline const int32_t &element(size_t i) const
    {
        return elements_.at(i);
    }

    inline int32_t &element(size_t i)
    {
        return elements_.at(i);
    }

    inline const int32_t &element(size_t x, size_t y) const
    {
        return elements_.at(y * n + x);
    }

    inline int32_t &element(size_t x, size_t y)
    {
        return elements_.at(y * n + x);
    }

    void init()
    {
        for (size_t i = 0; i < size; i++)
            element(i) = 0;
    }

    void print() const
    {
        for (size_t y = 0; y < m; y++)
        {
            for (size_t x = 0; x < n; x++)
            {
                std::print("{} ", element(x, y));
            }
            std::println();
        }
    }

    void random_fill()
    {
        // Create once when the function is invoked for the first time.
        static std::random_device seed;
        static std::mt19937 engine(seed());
        static std::uniform_int_distribution<> dist(-10, 10);

        for (size_t i = 0; i < size; i++)
            element(i) = dist(engine);
    }

    std::tuple<matrix, std::chrono::milliseconds, size_t> add(const matrix &other) const
    {
        size_t iterations = 0;
        auto pair = measure_time_ms(
            [&]
            {
                matrix result(m, n);
                for (size_t i = 0; i < size; i++)
                {
                    iterations += 1;

                    result.element(i) = element(i) + other.element(i);
                }
                return result;
            });

        return {pair.first, pair.second, iterations};
    }

    std::tuple<matrix, std::chrono::milliseconds, size_t> mult(const matrix &other) const
    {
        size_t iterations = 0;
        auto pair = measure_time_ms(
            [&]
            {
                matrix result(m, other.n);
                for (size_t y = 0; y < m; y++)
                {
                    for (size_t x = 0; x < other.n; x++)
                    {
                        // Calculate sum of products for the specific element.
                        int32_t sum = 0;
                        for (size_t j = 0; j < n; j++)
                        {
                            iterations += 1;

                            const int32_t product = element(j, y) * other.element(x, j);
                            sum += product;
                        }

                        result.element(x, y) = sum;
                    }
                }
                return result;
            });

        return {
            pair.first,
            pair.second,
            iterations,
        };
    }
};