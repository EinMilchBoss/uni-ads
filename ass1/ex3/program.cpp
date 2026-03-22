#include <print>
#include <random>

template <size_t M, size_t N>
class matrix
{
    std::vector<int32_t> elements_;

public:
    constexpr static size_t size = M * N;

    matrix() : elements_(size, 0)
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
        return elements_.at(y * N + x);
    }

    inline int32_t &element(size_t x, size_t y)
    {
        return elements_.at(y * N + x);
    }

    void init()
    {
        for (size_t i = 0; i < size; i++)
            element(i) = 0;
    }

    void print() const
    {
        for (size_t y = 0; y < M; y++)
        {
            for (size_t x = 0; x < N; x++)
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

    matrix<M, N> add(const matrix<M, N> &other) const
    {
        matrix<M, N> result;
        for (size_t i = 0; i < size; i++)
            result.element(i) = element(i) + other.element(i);
        return result;
    }

    template <size_t L>
    matrix<M, L> mult(const matrix<N, L> &other) const
    {
        matrix<M, L> result;
        for (size_t y = 0; y < M; y++)
        {
            for (size_t x = 0; x < L; x++)
            {
                // Calculate sum of products for the specific element.
                int32_t sum = 0;
                for (size_t j = 0; j < N; j++)
                {
                    const int32_t product = element(j, y) * other.element(x, j);
                    sum += product;
                }

                result.element(x, y) = sum;
            }
        }
        return result;
    }
};

int main()
{
    matrix<2, 3> m1{};
    m1.random_fill();
    m1.print();
    std::println();

    matrix<2, 3> m2{};
    m2.random_fill();
    m2.print();
    std::println();

    matrix<3, 4> m3{};
    m3.random_fill();
    m3.print();
    std::println();

    auto sum = m1.add(m2);
    sum.print();
    std::println();

    auto product = m1.mult(m3);
    product.print();
    std::println();

    return 0;
}