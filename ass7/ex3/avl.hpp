#include <cstdint>

class avl_tree
{
public:
    class vertex
    {
        friend class avl_tree;

        int val_;
        vertex *left_;
        vertex *right_;
        int8_t bal_;

    public:
        vertex(int val) noexcept;
    };

private:
    vertex *root_;

    vertex *rot_left_once(vertex *root);
    vertex *rot_right_once(vertex *root);
    vertex *rot_left_twice(vertex *root);
    vertex *rot_right_twice(vertex *root);

    vertex *rot_left(vertex *root);

public:
    avl_tree() noexcept;

    void _add(int val, vertex *curr);

    void add(int val);
};