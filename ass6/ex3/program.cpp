#include <iostream>
#include <cassert>
#include <sstream>
#include <random>
#include <stdexcept>

class node
{
    friend class ring_list;

    node *next_;
    int value_;

public:
    node(int value) noexcept
        : next_(nullptr),
          value_(value)
    {
    }

    node(const node &) = delete;

    node &operator=(const node &) = delete;

    node(node &&) = delete;

    node &operator=(node &&) = delete;

    ~node() = default;

    int value() const noexcept
    {
        return value_;
    }
};

class ring_list
{
    node *first_;
    size_t size_;

    /**
     * Returns the last node in our ring list.
     *
     * The caller has to make sure there is at least one element in our ring list.
     */
    node *last_unchecked() noexcept
    {
        assert(first_ != nullptr && "at least one node is in the ring list");
        node *current = first_;

        for (size_t i = 0; i < size_ - 1; i++)
        {
            assert(current->next_ != nullptr && "all nodes in ring list point to a next node");
            current = current->next_;
        }

        return current;
    }

    const node &at_internal(size_t idx) const
    {
        if (is_empty())
            throw std::out_of_range("empty ring list cannot be indexed");

        const size_t shortened_idx = idx % size_;

        node *current = first_;
        for (size_t i = 0; i < shortened_idx; i++)
        {
            current = current->next_;
        }
        return *current;
    }

public:
    ring_list()
        : first_(nullptr),
          size_(0)
    {
    }

    ring_list(const ring_list &) = delete;

    ring_list &operator=(const ring_list &) = delete;

    ring_list(ring_list &&) = delete;

    ring_list &operator=(ring_list &&) = delete;

    ~ring_list()
    {
        node *next = first_;
        for (size_t i = 0; i < size_ - 1; i++)
        {
            node *current = next;
            next = next->next_;
            delete current;
        }
        delete next;
    }

    const node &first() const noexcept
    {
        return *first_;
    }

    size_t size() const noexcept
    {
        return size_;
    }

    const node &at(size_t idx) const
    {
        return at_internal(idx);
    }

    node &at(size_t idx)
    {
        return const_cast<node &>(at_internal(idx));
    }

    void push_front(int value)
    {
        node *new_first = new node(value);

        if (is_empty())
        {
            assert(size_ == 0 && "an empty ring list has a size of 0");

            new_first->next_ = new_first;
            first_ = new_first;
            size_ = 1;
        }
        else
        {
            node *last = last_unchecked();

            new_first->next_ = first_;
            last->next_ = new_first;

            first_ = new_first;
            size_ += 1;
        }
    }

    int remove_at(size_t idx)
    {
        if (is_empty())
            throw std::out_of_range("nothing can be removed from empty ring list");

        const size_t predecessor_idx =
            idx == 0
                ? size_ - 1
                : idx - 1;

        if (size_ == 1)
        {
            int removed_value = first_->value_;
            delete first_;

            first_ = nullptr;
            size_ = 0;

            return removed_value;
        }
        else
        {
            assert(size_ >= 2 && "the zero and one node ring list cases have been covered already");

            node &predecessor = at(predecessor_idx);
            node *removed = predecessor.next_;
            predecessor.next_ = removed->next_;

            int removed_value = removed->value_;
            delete removed;
            size_ -= 1;

            return removed_value;
        }
    }

    bool is_empty() const noexcept
    {
        return first_ == nullptr;
    }

    std::string to_string() const noexcept
    {
        if (first_ == nullptr)
        {
            return "[]";
        }
        else
        {
            std::stringstream ss;
            ss << "[ *, ";

            node *current = first_;

            for (size_t i = 0; i < size_ - 1; i++)
            {
                ss << current->value_ << ", ";

                assert(current->next_ != nullptr && "all nodes in ring list point to a next node");
                current = current->next_;
            }

            ss << current->value_ << ", * ]";
            return ss.str();
        }
    }
};

int main()
{
    ring_list rl;
    for (int i = 1; i <= 49; i++)
        rl.push_front(i);

    std::cout << rl.to_string() << "\n";

    std::random_device rd;
    unsigned int seed = rd();
    std::mt19937 rng(seed);
    std::cout << "Using seed: " << seed << ".\n";

    size_t total_idx = 0;
    for (int i = 0; i < 6; i++)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, static_cast<uint_fast32_t>(rl.size() - 1));

        uint_fast32_t step = dist(rng);
        total_idx += step;

        const int picked_value = rl.remove_at(total_idx);
        std::cout << "Picked: " << picked_value << ".\n";
    }

    return 0;
}