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
};

class binary_search_tree
{
    vertex *root_;

    void do_add(vertex *new_vertex, vertex *current)
    {
        assert(new_vertex != nullptr && "we add a valid vertex");
        assert(current != nullptr && "we are inside our tree");

        if (new_vertex->value_ <= current->value_)
        {
            if (current->next_left_ == nullptr)
            {
                current->next_left_ = new_vertex;
                new_vertex->prev_ = current;
                return;
            }

            do_add(new_vertex, current->next_left_);
        }
        else
        {
            if (current->next_right_ == nullptr)
            {
                current->next_right_ = new_vertex;
                new_vertex->prev_ = current;
                return;
            }

            do_add(new_vertex, current->next_right_);
        }
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

    void add(int value)
    {
        vertex *new_vertex = new vertex(value);

        if (root_ == nullptr)
            root_ = new_vertex;
        else
            do_add(new_vertex, root_);
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
    // Binär verketteter Suchbaum
    // Löschen!

    binary_search_tree bst;
    bst.add(1);
    bst.add(2);
    bst.add(3);
    bst.add(0);
    bst.add(-2);
    bst.add(-1);
    std::cout << bst.to_string_in_order() << "\n";

    return 0;
}