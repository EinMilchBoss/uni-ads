#include <iostream>
#include <cassert>
#include <sstream>

class node
{
    friend class linked_list;

    node *next_;
    int value_;

    node(node *next, int value) noexcept
        : next_(next),
          value_(value)
    {
    }

    node(int value) noexcept
        : node(nullptr, value)
    {
    }

public:
    ~node() noexcept
    {
        delete next_;
    }

    const node *next() const noexcept
    {
        return next_;
    }

    int value() const noexcept
    {
        return value_;
    }
};

class linked_list
{
    node *first_;
    size_t size_;

    node *last_unchecked() noexcept
    {
        assert(first_ != nullptr && "there is no last element in linked list");

        node *last = first_;
        while (last->next_ != nullptr)
            last = last->next_;
        return last;
    }

    std::tuple<linked_list, int, linked_list> prepare_partitions_unchecked()
    {
        assert(size_ > 0 && "cannot choose a pivot element in empty list");
        int pivot_value = pop_front_unchecked();

        linked_list lefts, rights;

        node *current = first_;
        while (current != nullptr)
        {
            int current_value = current->value_;

            // `push_front` instead of `push_back`
            // to have O(1) instead of O(n).
            if (current_value <= pivot_value)
                lefts.push_front(current_value);
            else
                rights.push_front(current_value);

            current = current->next_;
        }

        return {std::move(lefts), pivot_value, std::move(rights)};
    }

public:
    linked_list()
        : first_(nullptr),
          size_(0)
    {
    }

    linked_list(const linked_list &) = delete;

    linked_list &&operator=(const linked_list &) = delete;

    linked_list(linked_list &&other) noexcept
        : first_(other.first_),
          size_(other.size_)
    {
        other.first_ = nullptr;
        other.size_ = 0;
    }

    linked_list &operator=(linked_list &&other) noexcept
    {
        if (this != &other)
        {
            // Sets off a chain reaction.
            delete first_;

            first_ = other.first_;
            size_ = other.size_;

            // Just to make sure.
            other.first_ = nullptr;
            other.size_ = 0;
        }

        return *this;
    }

    ~linked_list()
    {
        // Sets off a chain reaction.
        delete first_;

        // Just to leave the state cleanly.
        first_ = nullptr;
    }

    void push_back(int value)
    {
        node *new_last = new node(value);

        if (first_ == nullptr)
        {
            first_ = new_last;
        }
        else
        {
            node *old_last = last_unchecked();
            old_last->next_ = new_last;
        }

        size_ += 1;
    }

    void push_front(int value)
    {
        node *new_first = new node(first_, value);
        first_ = new_first;
        size_ += 1;
    }

    int pop_front_unchecked() noexcept
    {
        node *old_first = first_;
        int old_value = old_first->value_;

        // Set next to `nullptr` or else all nodes
        // are deleted recursively later on.
        first_ = old_first->next_;
        old_first->next_ = nullptr;

        delete old_first;
        size_ -= 1;

        return old_value;
    }

    void append(linked_list &&other) noexcept
    {
        if (first_ == nullptr)
        {
            first_ = other.first_;
            size_ = other.size_;
        }
        else
        {
            node *old_last = last_unchecked();
            old_last->next_ = other.first_;
            size_ += other.size_;
        }

        // Reset the other, just to make sure.
        other.first_ = nullptr;
        other.size_ = 0;
    }

    void quick_sort()
    {
        if (size_ <= 1)
            return;

        auto [lefts, pivot, rights] = prepare_partitions_unchecked();

        lefts.quick_sort();
        rights.quick_sort();

        lefts.push_back(pivot);
        lefts.append(std::move(rights));
        *this = std::move(lefts);
    }

    std::string to_string()
    {
        std::stringstream ss;

        node *current = first_;
        while (current != nullptr)
        {
            ss << current->value_;

            // Only append a comma if it's not the last.
            if (current->next_ != nullptr)
                ss << ", ";

            current = current->next_;
        }

        return ss.str();
    }
};

int main()
{
    linked_list ll;

    ll.push_front(4);
    ll.push_front(5);
    ll.push_front(3);
    ll.push_front(1);
    ll.push_front(2);

    std::cout << "original: " << ll.to_string() << '\n';

    ll.quick_sort();

    std::cout << "sorted: " << ll.to_string() << '\n';

    return 0;
}