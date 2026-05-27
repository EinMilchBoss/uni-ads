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
        size_t layer) const
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
    std::shared_ptr<node> find_predecessor(int32_t max_value, size_t max_layer) const
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
            for (; height < inserted->height_; ++height)
            {
                const size_t layer = last_layer - height;

                inserted->nexts_[layer] = roots_[layer];
                roots_[layer] = inserted;
            }
        }
        else
        {
            // Connect as much as possible.
            for (; height < inserted->height_ && height < pred->height_; ++height)
            {
                const size_t layer = last_layer - height;

                inserted->nexts_[layer] = pred->nexts_[layer];
                pred->nexts_[layer] = inserted;
            }

            // The current predecessor is smaller than the inserted node.
            // We have to find the next best predecessor and connect the rest.
            if (height < inserted->height_)
                do_insert(inserted, height);
        }
    }

    void do_remove(std::shared_ptr<node> removed, size_t height)
    {
        auto pred = find_predecessor(removed->value_, last_layer - height);
        if (pred == nullptr)
        {
            // Connect the roots to the inserted node for the rest.
            for (; height < removed->height_; ++height)
            {
                const size_t layer = last_layer - height;

                roots_[layer] = removed->nexts_[layer];
            }
        }
        else
        {
            // Connect as much as possible.
            for (; height < removed->height_ && height < pred->height_; ++height)
            {
                const size_t layer = last_layer - height;

                pred->nexts_[layer] = removed->nexts_[layer];
            }

            // The current predecessor is smaller than the inserted node.
            // We have to find the next best predecessor and connect the rest.
            if (height < removed->height_)
                do_remove(removed, height);
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

    void remove(int32_t value)
    {
        // Throw an error if the value does not exist.
        if (!contains(value))
            throw std::invalid_argument("tried to remove non-existing element in skip list");

        auto pred = find_predecessor(value, last_layer);
        auto removed = pred ? pred->nexts_[last_layer] : roots_[last_layer];
        assert(removed && "node is only a predecessor, if the next element is the one we look for");
        do_remove(removed, 0);
    }

    bool contains(int32_t value) const
    {
        const auto pred = find_predecessor(value, last_layer);

        if (pred)
        {
            return pred->nexts_[last_layer]
                       ? pred->nexts_[last_layer]->value_ == value
                       : false;
        }
        else
        {
            return roots_[last_layer]
                       ? roots_[last_layer]->value_ == value
                       : false;
        }
    }

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
    for (int32_t i = 0; i < 100; ++i)
    {
        sl.insert(i);
    }
    std::cout << sl << "\n";

    sl.remove(0);
    sl.remove(49);
    sl.remove(99);
    std::cout << sl << "\n";

    std::cout << " 0: " << !sl.contains(0) << "\n";
    std::cout << " 1: " << sl.contains(1) << "\n";

    std::cout << "49: " << !sl.contains(49) << "\n";
    std::cout << "50: " << sl.contains(50) << "\n";

    std::cout << "89: " << sl.contains(98) << "\n";
    std::cout << "99: " << !sl.contains(99) << "\n";

    return 0;
}