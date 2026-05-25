#include <iostream>
#include <random>
#include <memory>
#include <array>
#include <cassert>

constexpr size_t LAYERS = 5;

class skip_list
{
public:
    class node
    {
        friend class skip_list;

        std::array<std::shared_ptr<node>, LAYERS> nexts_ = {};
        size_t height_;
        int32_t value_;

    public:
        node(int32_t value, size_t height) noexcept
            : height_(height),
              value_(value)
        {
        }

        int32_t value() const noexcept
        {
            return value_;
        }
    };

private:
    constexpr static size_t last_layer = LAYERS - 1;

    std::array<std::shared_ptr<node>, LAYERS> roots_ = {};

    std::mt19937 rng_;
    std::uniform_int_distribution<uint8_t> dist_;

    bool flip_coin()
    {
        return dist_(rng_);
    }

    size_t won_coinflips()
    {
        for (size_t layer = 0; layer < last_layer; ++layer)
        {
            if (flip_coin())
                return layer + 1;
        }
        return LAYERS;
    }

    bool has_next_layer(size_t depth)
    {
        return depth < LAYERS;
    }

    std::shared_ptr<node> &last_lower(int32_t value, size_t layer)
    {
        if (roots_[layer] == nullptr)
            return roots_[layer];

        // Invariant: current is not null.
        // We use a pointer to avoid copies of the `shared_ptr`s.
        auto current = &roots_[layer];
        while (
            (*current)->nexts_[layer] != nullptr &&
            (*current)->nexts_[layer]->value_ < value)
        {
            current = &(*current)->nexts_[layer];
        }

        return *current;
    }

    void do_insert(int32_t value, std::shared_ptr<node> &inserted_prev)
    {
        assert(inserted_prev != nullptr);

        const size_t height = won_coinflips();
        auto inserted = std::make_shared<node>(value, height);

        size_t height_idx = 0;
        std::shared_ptr<node> *current_prev = &inserted_prev;
        while (true)
        {
            // Connect with the previous node as much as possible.
            while (height_idx < height && height_idx < (*current_prev)->height_)
            {
                const size_t layer = last_layer - height_idx;

                inserted->nexts_[layer] = (*current_prev)->nexts_[layer];
                (*current_prev)->nexts_[layer] = inserted;

                ++height_idx;
            }

            if (height_idx >= height)
                break;

            // In case the previous node is smaller than the current node,
            // we have to find the last lower (predecessor) of that layer instead.
            current_prev = &last_lower(value, last_layer - height_idx);
        }
    }

    void insert(int32_t value, std::shared_ptr<node> &current, size_t layer)
    {
        assert(current != nullptr);

        if (current->nexts_[layer] == nullptr || value <= current->nexts_[layer]->value_)
        {
            if (layer < last_layer)
            {
                // Move down if the next node is bigger or doesn't exist and there is a next layer.
                insert(value, current, layer + 1);
            }
            else
            {
                // Add new node to the right.
                do_insert(value, current);
            }
        }
        else
        {
            // Move on in the same layer.
            insert(value, current->nexts_[layer], layer);
        }
    }

public:
    skip_list(std::random_device::result_type seed)
        : rng_(std::mt19937(seed)),
          dist_(0, 1)
    {
    }

    void insert(int32_t value)
    {
        for (size_t start_layer = 0; start_layer < LAYERS; ++start_layer)
        {
            if (roots_[start_layer] != nullptr && roots_[start_layer]->value_ < value)
            {
                insert(value, roots_[start_layer], start_layer);
                return;
            }
        }

        // The node is either the first one or the smallest so far.
        const size_t height = won_coinflips();
        auto inserted = std::make_shared<node>(value, height);

        for (size_t i = 0; i < height; ++i)
        {
            const size_t layer = last_layer - i;

            inserted->nexts_[layer] = roots_[layer];
            roots_[layer] = inserted;
        }
    }

    // void remove(int32_t value)
    // {
    // }

    // bool contains(int32_t value)
    // {
    // }
};

int main()
{
    std::random_device generate_seed;
    auto seed = generate_seed();
    std::cout << "Used seed: " << seed << ".\n";

    skip_list sl(seed);
    sl.insert(1);
    sl.insert(2);
    sl.insert(0);
    // sl.contains(1);
    // sl.remove(1);

    return 0;
}