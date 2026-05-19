#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

template <typename V>
class min_heap
{
    std::vector<V> values_;

    size_t get_min_value_idx(const size_t l_idx, const size_t r_idx) const noexcept
    {
        if (values_[l_idx] <= values_[r_idx])
            return l_idx;
        else
            return r_idx;
    }

    void sift_down_unchecked(const size_t curr_idx)
    {
        assert(curr_idx < values_.size() && "tried to sift down invalid index of min heap");

        const size_t next_l_idx = 2 * curr_idx + 1;
        const size_t next_r_idx = next_l_idx + 1;

        if (next_r_idx < values_.size())
        {
            // We have two successors.

            const size_t min_idx = get_min_value_idx(next_l_idx, next_r_idx);
            if (values_[curr_idx] <= values_[min_idx])
                return;

            std::swap(values_[curr_idx], values_[min_idx]);
            sift_down_unchecked(min_idx);
        }
        else if (next_l_idx < values_.size())
        {
            // We have one successor.

            if (values_[curr_idx] <= values_[next_l_idx])
                return;

            std::swap(values_[curr_idx], values_[next_l_idx]);
            sift_down_unchecked(next_l_idx);
        }
    }

    void bubble_up_unchecked(const size_t curr_idx)
    {
        assert(curr_idx < values_.size() && "tried to bubble up invalid index of min heap");

        if (curr_idx == 0)
            return;

        const size_t prev_idx = (curr_idx - 1) / 2;

        if (values_[curr_idx] < values_[prev_idx])
        {
            std::swap(values_[curr_idx], values_[prev_idx]);
            bubble_up_unchecked(prev_idx);
        }
    }

    size_t last_parent_idx_unchecked() const noexcept
    {
        assert(values_.size() >= 2 && "there is no parent");

        return (values_.size() - 2) / 2;
    }

    void heapify()
    {
        if (values_.size() <= 1)
            return;

        for (size_t i = last_parent_idx_unchecked(); i > 0; i--)
            sift_down_unchecked(i);
        sift_down_unchecked(0);
    }

public:
    const V &top() const
    {
        if (values_.size() == 0)
            throw std::out_of_range("tried to get top element of empty min heap");

        return values_[0];
    }

    size_t size() const noexcept
    {
        return values_.size();
    }

    void add(V value)
    {
        values_.push_back(value);
        bubble_up_unchecked(values_.size() - 1);
    }

    V pop()
    {
        if (values_.size() == 0)
            throw std::out_of_range("tried to pop non-existing element of priority queue");

        std::swap(values_.front(), values_.back());
        const V popped = std::move(values_.back());
        values_.pop_back();
        sift_down_unchecked(0);

        return popped;
    }

    void update(size_t idx, V new_value)
    {
        if (idx >= values_.size())
            throw std::out_of_range("tried to update invalid index of priority queue");

        const V old_value = std::move(values_[idx]);
        values_[idx] = new_value;

        if (old_value < new_value)
            sift_down_unchecked(idx);
        else if (old_value > new_value)
            bubble_up_unchecked(idx);
    }

    const V &operator[](size_t idx) const noexcept
    {
        return values_[idx];
    }

    friend std::ostream &operator<<(std::ostream &s, min_heap heap)
    {
        if (heap.size() == 0)
            return s << "[]";

        s << '[' << heap.values_[0];
        for (size_t i = 1; i < heap.values_.size(); i++)
            s << ", " << heap.values_[i];
        s << ']';
        return s;
    }
};

template <typename K, typename V>
class priority_queue
{
public:
    class item
    {
    private:
        K priority_;
        V value_;

    public:
        item(K priority, V value) noexcept
            : priority_{priority},
              value_{value}
        {
        }

        K priority() const noexcept
        {
            return value_;
        }

        V value() const noexcept
        {
            return value_;
        }

        V &value() noexcept
        {
            return value_;
        }

        bool operator==(const item &i) const noexcept
        {
            return priority_ == i.priority_;
        }

        bool operator<(const item &i) const noexcept
        {
            return priority_ < i.priority_;
        }

        bool operator>(const item &i) const noexcept
        {
            return priority_ > i.priority_;
        }

        bool operator<=(const item &i) const noexcept
        {
            return *this == i || *this < i;
        }

        bool operator>=(const item &i) const noexcept
        {
            return *this == i || *this > i;
        }
    };

private:
    min_heap<item> min_heap_;

public:
    size_t size() const noexcept
    {
        return min_heap_.size();
    }

    void add(const int32_t value, const K priority)
    {
        min_heap_.add(item{priority, value});
    }

    const item &min() const
    {
        return min_heap_.top();
    }

    item pop_min()
    {
        return min_heap_.pop();
    }

    void change_key(const size_t idx, const K priority)
    {
        if (idx >= min_heap_.size())
            throw std::out_of_range("tried to decrease invalid index of priority queue");

        min_heap_.update(idx, item{priority, min_heap_[idx].value()});
    }

    friend std::ostream &operator<<(std::ostream &s, const priority_queue &pq)
    {
        if (pq.size() == 0)
            return s << "[]";

        s << '[' << pq.min_heap_[0].value();
        for (size_t i = 1; i < pq.size(); i++)
            s << ", " << pq.min_heap_[i].value();
        s << ']';
        return s;
    }
};

int main()
{
    priority_queue<int32_t, int32_t> pq;

    pq.add(5, 5);
    pq.add(1, 1);
    pq.add(4, 4);
    pq.add(2, 2);
    pq.add(3, 3);
    pq.add(6, 6);

    std::cout << pq << "\n";

    std::cout << "Min: " << pq.min().value() << "\n";

    auto min_item = pq.pop_min();

    std::cout << "Extracted min: " << min_item.value() << "\n";
    std::cout << "New min: " << pq.min().value() << "\n";
    std::cout << pq << "\n";

    pq.change_key(2, 1);
    std::cout << pq << "\n";

    pq.change_key(0, 100);
    std::cout << pq << "\n";

    pq.add(7, 7);
    std::cout << pq << "\n";

    return 0;
}