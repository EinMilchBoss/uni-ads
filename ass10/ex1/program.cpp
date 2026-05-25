#include <iostream>
#include <random>
#include <memory>
#include <array>
#include <cassert>

class skip_list
{
public:
    constexpr static size_t layer_amount = 5;
    constexpr static size_t last_layer = layer_amount - 1;

    class node
    {
        friend class skip_list;

        std::array<std::shared_ptr<node>, layer_amount> nexts_ = {};
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
    std::array<std::shared_ptr<node>, layer_amount> roots_ = {};

    std::mt19937 rng_;
    std::uniform_int_distribution<uint8_t> dist_{0, 1};

    bool flip_coin()
    {
        return dist_(rng_);
    }

    size_t get_random_height()
    {
        for (size_t layer = 1; layer < layer_amount; ++layer)
        {
            if (flip_coin())
                return layer;
        }
        return layer_amount;
    }

    /**
     * `max_value` is exclusive and `max_layer` is inclusive.
     */
    std::shared_ptr<node> do_find_predecessor(
        int32_t max_value,
        size_t max_layer,
        std::shared_ptr<node> current,
        size_t layer)
    {
        if (current->nexts_[layer] != nullptr && current->nexts_[layer]->value_ < max_value)
        {
            // Move on in the same layer.
            return do_find_predecessor(max_value, max_layer, current->nexts_[layer], layer);
        }
        else
        {
            if (layer < max_layer)
            {
                // Move down if the next node is bigger or doesn't exist and there is a next layer.
                return do_find_predecessor(max_value, max_layer, current, layer + 1);
            }
            else
            {
                // We reached the end.
                return current;
            }
        }
    }

    /**
     * `max_value` is exclusive and `max_layer` is inclusive.
     */
    std::shared_ptr<node> find_predecessor(int32_t max_value, size_t max_layer)
    {
        for (size_t i = 0; i <= max_layer; ++i)
        {
            if (roots_[i] != nullptr && roots_[i]->value_ < max_value)
                return do_find_predecessor(max_value, max_layer, roots_[i], i);
        }
        return nullptr;
    }

    void do_insert(std::shared_ptr<node> inserted, size_t height)
    {
        auto pred = find_predecessor(inserted->value_, last_layer - height);
        if (pred == nullptr)
        {
            // Connect the roots to the inserted node for the rest.
            while (height < inserted->height_)
            {
                const size_t layer = last_layer - height;

                inserted->nexts_[layer] = roots_[layer];
                roots_[layer] = inserted;
                ++height;
            }
        }
        else
        {
            // Connect as much as possible.
            while (height < inserted->height_ && height < pred->height_)
            {
                const size_t layer = last_layer - height;

                inserted->nexts_[layer] = pred->nexts_[layer];
                pred->nexts_[layer] = inserted;
                ++height;
            }

            // The current predecessor is smaller than the inserted node.
            // We have to find the next best predecessor and connect the rest.
            if (height < inserted->height_)
                do_insert(inserted, height);
        }
    }

public:
    skip_list(std::random_device::result_type seed)
        : rng_(std::mt19937(seed))
    {
    }

    void insert(int32_t value)
    {
        const size_t height = get_random_height();
        auto inserted = std::make_shared<node>(value, height);

        do_insert(inserted, 0);
    }

    // void remove(int32_t value)
    // {
    // }

    // bool contains(int32_t value)
    // {
    // }

    void output_layer(std::ostream &s, size_t layer) const
    {
        const std::shared_ptr<node> *current = &roots_[layer];
        if (*current == nullptr)
            return;

        s << (*current)->value_;
        current = &(*current)->nexts_[layer];

        while (*current)
        {
            s << ", " << (*current)->value_;
            current = &(*current)->nexts_[layer];
        }
    }

    friend std::ostream &operator<<(std::ostream &s, const skip_list &sl)
    {
        for (size_t i = 0; i < last_layer; ++i)
        {
            s << i << ": (";
            sl.output_layer(s, i);
            s << "), ";
        }

        s << last_layer << ": (";
        sl.output_layer(s, last_layer);
        s << ')';

        return s;
    }
};

int main()
{
    std::random_device generate_seed;
    auto seed = generate_seed();
    std::cout << "Used seed: " << seed << ".\n";

    skip_list sl(seed);
    for (int32_t i = 0; i < 10; ++i)
    {
        sl.insert(i);
    }

    std::cout << sl << "\n";

    return 0;
}