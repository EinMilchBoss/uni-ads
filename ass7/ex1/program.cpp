#include <iostream>
#include <cassert>
#include <memory>
#include <sstream>

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

    void perform_replacement(vertex *replacee, vertex **prev_next)
    {
        if (replacee->next_left_ == nullptr && replacee->next_right_ == nullptr)
        {
            // If the replacee doesn't have any subtrees, just remove the vertex and update previous vertex.

            *prev_next = nullptr;
            delete replacee;
        }
        else if (replacee->next_left_ != nullptr && replacee->next_right_ != nullptr)
        {
            // If the replacee has two subtrees, find the in-order predecessor.

            vertex *replacer = leftmost(replacee->next_right_);

            if (replacer == replacee->next_right_)
            {
                replacer->prev_ = replacee->prev_;
                replacer->next_left_ = replacee->next_left_;
                *prev_next = replacer;
                delete replacee;
            }
            else
            {
                replacee->value_ = replacer->value_;
                replacer->prev_->next_left_ = nullptr;
                delete replacer;
            }
        }
        else
        {
            // If the replacee has only one subtree, connect that subtree directly.

            vertex *replacer =
                replacee->next_left_ != nullptr
                    ? replacee->next_left_
                    : replacee->next_right_;

            replacer->prev_ = replacee->prev_;
            *prev_next = replacer;
            delete replacee;
        }
    }

    void do_remove(int value, vertex *current, vertex **prev_next)
    {
        assert(prev_next != nullptr && *prev_next == current && "the successor of the previous vertex is the current vertex");

        // The value does not exist in our tree.
        if (current == nullptr)
            return;

        if (value < current->value_)
            do_remove(value, current->next_left_, &current->next_left_);
        else if (value > current->value_)
            do_remove(value, current->next_right_, &current->next_right_);
        else
            perform_replacement(current, prev_next);
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

    vertex *leftmost(vertex *current)
    {
        assert(current != nullptr && "we are inside our tree");

        if (current->next_left_ == nullptr)
            return current;
        else
            return leftmost(current->next_left_);
    }

public:
    binary_search_tree() noexcept
        : root_(nullptr)
    {
    }

    binary_search_tree(const binary_search_tree &) = delete;
    binary_search_tree &operator=(const binary_search_tree &) = delete;
    binary_search_tree(binary_search_tree &&) = delete;
    binary_search_tree &operator=(binary_search_tree &&) = delete;
    ~binary_search_tree() = default;

    void add(int value)
    {
        vertex *new_vertex = new vertex(value);

        if (root_ == nullptr)
            root_ = new_vertex;
        else
            do_add(new_vertex, root_);
    }

    /**
     * Worst case runtime complexity for degenerated tree.
     * This is not divide and conquer, because we only follow one branch at a time.
     * `perform_replacement()` is Θ(1).
     *
     * T(0) = 1
     * T(n) = T(n - 1) + Θ(1)
     * => Θ(n)
     */
    void remove_rec(int value)
    {
        do_remove(value, root_, &root_);
    }

    /**
     * Worst case runtime complexity for degenerated tree.
     * Like for a doubly linked list we go through all elements until we reach the last element to remove it.
     * `perform_replacement()` is Θ(1).
     * The total time complexity is therefore Θ(n).
     */
    void remove_iter(int value)
    {
        vertex **prev_next = &root_;
        vertex *current = root_;

        while (current != nullptr && current->value_ != value)
        {
            prev_next =
                value < current->value_
                    ? &current->next_left_
                    : &current->next_right_;
            current = *prev_next;
        }

        // The value does not exist in our tree.
        if (current == nullptr)
            return;

        perform_replacement(current, prev_next);
    }

    std::string to_string_in_order() const
    {
        std::stringstream ss;
        do_to_string_in_order(ss, root_);
        return ss.str();
    }
};

int main()
{
    binary_search_tree bst;
    bst.add(1);
    bst.add(2);
    bst.add(3);
    bst.add(0);
    bst.add(-2);
    bst.add(-1);

    bst.remove_rec(3);
    bst.remove_rec(2);
    bst.remove_rec(-2);
    bst.remove_rec(0);
    bst.remove_rec(1);
    bst.remove_rec(-1);
    bst.remove_rec(1);

    bst.remove_iter(3);
    bst.remove_iter(2);
    bst.remove_iter(-2);
    bst.remove_iter(0);
    bst.remove_iter(1);
    bst.remove_iter(-1);
    bst.remove_iter(1);

    std::cout << bst.to_string_in_order() << "\n";

    return 0;
}