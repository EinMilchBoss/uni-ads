#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

template <typename T>
class min_heap
{
    std::vector<T> values_;

    size_t get_min_value_idx(size_t l_idx, size_t r_idx)
    {
        if (values_[l_idx] <= values_[r_idx])
            return l_idx;
        else
            return r_idx;
    }

    void sift_down_unchecked(const size_t curr_idx) noexcept
    {
        std::cout << curr_idx << " / " << values_.size() << std::endl;
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

    void bubble_up_unchecked(const size_t curr_idx) noexcept
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
    const T &top() const
    {
        if (values_.size() == 0)
            throw std::out_of_range("tried to get top element of empty min heap");

        return values_[0];
    }

    size_t size() const noexcept
    {
        return values_.size();
    }

    void add(T value)
    {
        values_.push_back(value);
        heapify();
    }

    T pop()
    {
        T top = values_[0];

        std::swap(values_[0], values_[values_.size() - 1]);
        values_.pop_back();
        sift_down_unchecked(0);

        return top;
    }

    void update(size_t idx, T new_value)
    {
        T old_value = values_[idx];
        values_[idx] = new_value;

        if (old_value < new_value)
            sift_down_unchecked(idx);
        else if (old_value > new_value)
            bubble_up_unchecked(idx);
    }

    const T &operator[](size_t idx) const
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

class priority_queue
{
public:
    class item
    {
    public:
        using priority_type = int32_t;

    private:
        priority_type priority_;
        int32_t value_;

    public:
        item(priority_type priority, int32_t value) noexcept
            : priority_{priority},
              value_{value}
        {
        }

        priority_type priority() const noexcept
        {
            return value_;
        }

        int32_t value() const noexcept
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

    void insert(int32_t value, item::priority_type priority)
    {
        min_heap_.add(item{priority, value});
    }

    const item &get_min()
    {
        return min_heap_.top();
    }

    item extract_min()
    {
        return min_heap_.pop();
    }

    void change_key(size_t idx, item::priority_type priority)
    {
        if (idx >= min_heap_.size())
            throw std::out_of_range("tried to decrease invalid index of priority queue");

        min_heap_.update(idx, item{priority, min_heap_[idx].value()});
    }

    friend std::ostream &operator<<(std::ostream &s, const priority_queue &pq) noexcept
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
    priority_queue pq;

    pq.insert(5, 5);
    pq.insert(1, 1);
    pq.insert(4, 4);
    pq.insert(2, 2);
    pq.insert(3, 3);
    pq.insert(6, 6);

    std::cout << pq << "\n";

    std::cout << "Min: " << pq.get_min().value() << "\n";

    priority_queue::item min_item = pq.extract_min();

    std::cout << "Extracted min: " << min_item.value() << "\n";
    std::cout << "New min: " << pq.get_min().value() << "\n";
    std::cout << pq << "\n";

    pq.change_key(2, 1);
    std::cout << pq << "\n";

    pq.change_key(0, 100);
    std::cout << pq << "\n";

    return 0;
}