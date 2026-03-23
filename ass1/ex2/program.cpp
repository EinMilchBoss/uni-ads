#include <print>
#include <vector>

uint32_t sqrt(uint32_t n)
{
    return static_cast<uint32_t>(std::sqrt(static_cast<double>(n)));
}

std::vector<uint32_t> sieve_primes(uint32_t k)
{
    // Indices go from 0 to k. We won't use 0 and 1, but that is alright.
    bool *sieve = new bool[k + 1]{0};

    uint32_t end_inclusive = sqrt(k);
    for (uint32_t i = 2; i <= end_inclusive; i++)
    {
        // If number has not yet been crossed out, remove all multiples.
        if (!sieve[i])
        {
            for (uint32_t j = 2 * i; j <= k; j += i)
                sieve[j] = true;
        }
    }

    // Add all left-over primes and free memory.
    std::vector<uint32_t> primes{};
    for (uint32_t i = 2; i <= k; i++)
    {
        if (!sieve[i])
            primes.push_back(i);
    }
    delete[] sieve;

    return primes;
}

int main()
{
    auto primes = sieve_primes(100000);
    for (std::size_t i = 0; i < primes.size(); i++)
    {
        std::println("[{:>4}] {}", i + 1, primes[i]);
    }
}