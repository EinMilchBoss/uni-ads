#include <iostream>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "rng.hpp"

/**
 * Return the first index of the provided value in the orignal array.
 *
 * This function throws an exception if the value doesn't exist at all.
 */
int first_index_of_value(const int *arr, const int len, const int value)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == value)
            return i;
    }

    throw std::runtime_error("could not find index of value in array");
}

class vertex
{
    friend class binary_search_tree;

    int value_;
    vertex *prev_;
    vertex *next_left_;
    vertex *next_right_;

public:
    vertex(int value) noexcept
        : value_(value),
          next_left_(nullptr),
          next_right_(nullptr)
    {
    }

    vertex(const vertex &) = delete;
    vertex &operator=(const vertex &) = delete;
    vertex(vertex &&) = delete;
    vertex &operator=(vertex &&) = delete;
    ~vertex() = default;
};

class binary_search_tree
{
    vertex *root_;

    binary_search_tree(vertex *root) noexcept
        : root_(root)
    {
    }

    void do_add(vertex *new_vertex, vertex *current)
    {
        assert(new_vertex != nullptr && "we add a valid vertex");
        assert(current != nullptr && "we are inside our tree");

        if (new_vertex->value_ < current->value_)
        {
            if (current->next_left_ == nullptr)
            {
                current->next_left_ = new_vertex;
                new_vertex->prev_ = current;
                return;
            }

            do_add(new_vertex, current->next_left_);
        }
        else if (new_vertex->value_ > current->value_)
        {
            if (current->next_right_ == nullptr)
            {
                current->next_right_ = new_vertex;
                new_vertex->prev_ = current;
                return;
            }

            do_add(new_vertex, current->next_right_);
        }
        else
        {
            throw std::runtime_error("the same number was inserted into the binary search tree twice");
        }
    }

    void do_to_pre_order_list(std::vector<int> &result, const vertex *current) const
    {
        if (current == nullptr)
            return;

        result.push_back(current->value_);
        do_to_pre_order_list(result, current->next_left_);
        do_to_pre_order_list(result, current->next_right_);
    }

    void do_to_in_order_list(std::vector<int> &result, const vertex *current) const
    {
        if (current == nullptr)
            return;

        do_to_in_order_list(result, current->next_left_);
        result.push_back(current->value_);
        do_to_in_order_list(result, current->next_right_);
    }

    void do_to_string_pre_order(std::stringstream &ss, const vertex *current) const
    {
        if (current == nullptr)
        {
            ss << "null";
            return;
        }

        ss << "( " << current->value_ << ", ";
        do_to_string_pre_order(ss, current->next_left_);
        ss << ", ";
        do_to_string_pre_order(ss, current->next_right_);
        ss << " )";
    }

    void do_to_string_in_order(std::stringstream &ss, const vertex *current) const
    {
        if (current == nullptr)
        {
            ss << "null";
            return;
        }

        ss << "( ";
        do_to_string_in_order(ss, current->next_left_);
        ss << ", " << current->value_ << ", ";
        do_to_string_in_order(ss, current->next_right_);
        ss << " )";
    }

public:
    binary_search_tree() noexcept
        : root_(nullptr)
    {
    }

    binary_search_tree(const binary_search_tree &) = delete;
    binary_search_tree &operator=(const binary_search_tree &) = delete;

    binary_search_tree(binary_search_tree &&other)
    {
        root_ = other.root_;
        other.root_ = nullptr;
    }

    binary_search_tree &operator=(binary_search_tree &&) = delete;

    ~binary_search_tree()
    {
        // TODO
    }

    static vertex *recurse(
        const int *pre,
        const int *in,
        const int len,
        const int in_first_idx,
        const int in_last_idx,
        int &pre_idx)
    {
        pre_idx += 1;
        const int value = pre[pre_idx];

        if (in_first_idx == in_last_idx)
            return new vertex(value);
        else if (in_first_idx > in_last_idx)
        {
            pre_idx -= 1;
            return nullptr;
        }
        vertex *new_vertex = new vertex(value);

        const int idx_offset = first_index_of_value(in + in_first_idx, in_last_idx - in_first_idx + 1, value);

        vertex *left = recurse(pre, in, len, in_first_idx, in_first_idx + idx_offset - 1, pre_idx);

        vertex *right = recurse(pre, in, len, in_first_idx + idx_offset + 1, in_last_idx, pre_idx);

        new_vertex->next_left_ = left;
        new_vertex->next_right_ = right;

        return new_vertex;
    }

    static binary_search_tree from_pre_and_in_order(const int *pre, const int *in, const int len)
    {
        int pre_idx = -1;
        binary_search_tree bst(recurse(pre, in, len, 0, len - 1, pre_idx));
        return bst;
    }

    void add(int value)
    {
        vertex *new_vertex = new vertex(value);

        if (root_ == nullptr)
            root_ = new_vertex;
        else
            do_add(new_vertex, root_);
    }

    std::vector<int> to_pre_order_list() const
    {
        std::vector<int> result;
        do_to_pre_order_list(result, root_);
        return result;
    }

    std::vector<int> to_in_order_list() const
    {
        std::vector<int> result;
        do_to_in_order_list(result, root_);
        return result;
    }

    std::string to_string_pre_order() const
    {
        std::stringstream ss;
        do_to_string_pre_order(ss, root_);
        return ss.str();
    }

    std::string to_string_in_order() const
    {
        std::stringstream ss;
        do_to_string_in_order(ss, root_);
        return ss.str();
    }
};

bool run_test(unsigned int seed)
{
    std::mt19937 rng(seed);

    binary_search_tree original;
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7};
    for (int j = 0; j < 7; j++)
    {
        std::uniform_int_distribution<size_t> dist(0, values.size() - 1);
        const size_t idx = dist(rng);

        original.add(values[idx]);
        values.erase(values.begin() + static_cast<ptrdiff_t>(idx));
    }

    try
    {
        auto pre = original.to_pre_order_list();
        auto in = original.to_in_order_list();
        assert(pre.size() == in.size() && "all different orders of the same bst have the same length");

        binary_search_tree reconstructed = binary_search_tree::from_pre_and_in_order(pre.data(), in.data(), static_cast<int>(in.size()));

        auto original_in_str = original.to_string_in_order();
        auto reconstructed_in_str = reconstructed.to_string_in_order();

        if (original_in_str != reconstructed_in_str)
        {
            std::cerr << "The following test case failed:\n";
            std::cerr << "original (pre): " << original.to_string_pre_order() << "\n";
            std::cerr << "original (in):  " << original_in_str << "\n";
            std::cerr << "reconstructred: " << reconstructed_in_str << "\n";
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error occurred for pre:\n";
        for (const int x : original.to_pre_order_list())
            std::cout << x << " ";

        return false;
    }
}

int main()
{
    binary_search_tree bst;
    bst.add(4);
    bst.add(2);
    bst.add(6);
    bst.add(1);
    bst.add(3);
    bst.add(5);
    bst.add(7);

    std::cout << "old in-order:  " << bst.to_string_in_order() << "\n";
    std::cout << "old pre-order: " << bst.to_string_pre_order() << "\n";

    std::vector<int> in = bst.to_in_order_list();
    for (const int x : in)
        std::cout << x << " ";
    std::cout << "\n";

    std::vector<int> pre = bst.to_pre_order_list();
    for (const int x : pre)
        std::cout << x << " ";
    std::cout << "\n";

    assert(pre.size() == in.size() && "all different orders of the same bst have the same length");
    auto new_bst = binary_search_tree::from_pre_and_in_order(pre.data(), in.data(), static_cast<int>(in.size()));
    std::cout << "new in-order:  " << new_bst.to_string_in_order() << "\n";
    std::cout << "new pre-order: " << new_bst.to_string_pre_order() << "\n";

    // Perform randomized tests.
    std::random_device seed_generator;
    // TODO: unsigned int seed = seed_generator();
    unsigned int seed = 2166346741;
    rng rng(0, 9, seed);
    std::cout << "Using random seed " << seed << ".\n";

    for (int i = 0; i < 10000; i++)
    {
        if (!run_test(seed))
            return 1;

        std::cout << "Passed test " << i << ".\n";
    }

    std::cout << "All tests passed.\n";
    return 0;
}