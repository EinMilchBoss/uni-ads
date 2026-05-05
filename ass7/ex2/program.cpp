#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <random>
#include <cstdint>
#include <cassert>

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

    void delete_including_subtrees(vertex *parent)
    {
        if (parent == nullptr)
            return;

        delete_including_subtrees(parent->next_left_);
        delete_including_subtrees(parent->next_right_);
        delete parent;
    }

    static vertex *do_from_pre_and_in_order(
        const int *pre,
        const int *in,
        const int len,
        const int in_first_idx,
        const int in_last_idx,
        int &pre_idx)
    {
        if (in_first_idx > in_last_idx)
            return nullptr;

        pre_idx += 1;
        const int value = pre[pre_idx];

        const int idx_offset = first_index_of_value(in + in_first_idx, in_last_idx - in_first_idx + 1, value);
        vertex *left = do_from_pre_and_in_order(pre, in, len, in_first_idx, in_first_idx + idx_offset - 1, pre_idx);
        vertex *right = do_from_pre_and_in_order(pre, in, len, in_first_idx + idx_offset + 1, in_last_idx, pre_idx);

        vertex *new_vertex = new vertex(value);
        new_vertex->next_left_ = left;
        new_vertex->next_right_ = right;

        return new_vertex;
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

    ~binary_search_tree() noexcept
    {
        delete_including_subtrees(root_);
    }

    static binary_search_tree from_pre_and_in_order(const int *pre, const int *in, const int len)
    {
        int pre_idx = -1;
        binary_search_tree bst(do_from_pre_and_in_order(pre, in, len, 0, len - 1, pre_idx));
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

/**
 * This function creates a binary search tree of size n with non-repeating values from [1, n].
 */
binary_search_tree create_random_bst(const size_t n, const unsigned int seed)
{
    binary_search_tree original;

    std::vector<int> values;
    values.reserve(n);
    for (size_t i = 1; i <= n; i++)
        values.push_back(static_cast<int>(i));

    std::mt19937 rng(seed);
    for (size_t j = 0; j < n; j++)
    {
        std::uniform_int_distribution<size_t> dist(0, values.size() - 1);
        const size_t rnd_idx = dist(rng);

        original.add(values[rnd_idx]);
        values.erase(values.begin() + static_cast<ptrdiff_t>(rnd_idx));
    }

    return original;
}

/**
 * This function returns a string containing all values of the passed vector separated by space.
 */
std::string vector_to_string(const std::vector<int> &v)
{
    if (v.size() == 0)
        return std::string{};

    std::stringstream ss{};
    ss << v[0];
    for (size_t i = 1; i < v.size(); i++)
        ss << ' ' << v[i];
    return ss.str();
}

int main()
{
    std::random_device seed_generator;
    const unsigned int seed = seed_generator();
    std::cout << "Using random seed " << seed << ".\n";

    constexpr size_t TEST_RUNS = 1'000'000;
    constexpr size_t BST_SIZE = 1000;

    for (size_t i = 0; i < TEST_RUNS; i++)
    {
        const binary_search_tree expected = create_random_bst(BST_SIZE, seed);
        const std::vector<int> expected_pre = expected.to_pre_order_list();
        const std::vector<int> expected_in = expected.to_in_order_list();

        assert(expected_pre.size() == expected_in.size() && "all different orders of the same bst have the same length");

        const binary_search_tree actual = binary_search_tree::from_pre_and_in_order(expected_pre.data(), expected_in.data(), static_cast<int>(expected_in.size()));
        const std::vector<int> actual_pre = actual.to_pre_order_list();
        const std::vector<int> actual_in = actual.to_in_order_list();

        // The in-order of a BST is always sorted if correct.
        if (expected_pre == actual_pre && expected_in == actual_in)
        {
            std::cout << "Passed test " << i << ".\n";
        }
        else
        {
            const std::string expected_pre_str = vector_to_string(expected_pre);
            const std::string actual_pre_str = vector_to_string(actual_pre);

            std::cerr << "Test " << i << " failed:\n";
            std::cerr << "expected (in): " << expected_pre_str << "\n";
            std::cerr << "actual   (in): " << expected_pre_str << "\n";

            return 1;
        }
    }

    std::cout << "All tests passed.\n";
    return 0;
}