#pragma once

#include <random>
#include <cstdint>

class rng
{
    static std::random_device seed_generator_;

    int32_t from_;
    int32_t to_;
    unsigned int seed_;

    std::mt19937 engine_;
    std::uniform_int_distribution<int32_t> distribution_;

public:
    rng(int32_t from, int32_t to, unsigned int seed)
        : from_(from),
          to_(to),
          seed_(seed)
    {
        if (from > to)
            throw std::invalid_argument("random number generator range is invalid");

        engine_ = std::mt19937(seed);
        distribution_ = std::uniform_int_distribution<int32_t>(from, to);
    }

    rng(int32_t from, int32_t to) : rng(from, to, seed_generator_())
    {
    }

    int32_t from() const noexcept
    {
        return from_;
    }

    int32_t to() const noexcept
    {
        return to_;
    }

    unsigned int seed() const noexcept
    {
        return seed_;
    }

    int32_t next() noexcept
    {
        return distribution_(engine_);
    }
};