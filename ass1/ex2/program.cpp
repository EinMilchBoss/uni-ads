#include <print>
#include <vector>

uint32_t sqrt(uint32_t n)
{
    return static_cast<uint32_t>(std::sqrt(static_cast<double>(n)));
}

bool is_prime(uint32_t suspect)
{
    uint32_t end_inclusive = sqrt(suspect);
    for (uint32_t i = 2; i <= end_inclusive; i++)
    {
        if (suspect % i == 0)
            return false;
    }
    return true;
}

std::vector<uint32_t> sieve_primes(uint32_t k)
{
    // Indices go from 0 to k. We won't use 0 and 1, but that is alright.
    bool *sieve = new bool[k + 1]{0};

    uint32_t end_inclusive = sqrt(k);
    for (uint32_t i = 2; i <= end_inclusive; i++)
    {
        // If number is already marked as non-prime, skip this iteration.
        if (sieve[i])
            continue;

        // If number is not marked yet and happens to be a prime, cross out multiples.
        if (is_prime(i))
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