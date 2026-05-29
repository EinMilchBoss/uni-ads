#include <iostream>
#include <cstdint>

enum class balance : uint8_t
{
    balanced = 0b00,
    left = 0b10,
    right = 0b01
};

class vertex
{
    friend class avl_tree;

    int val_;
    vertex *left_;
    vertex *right_;
    balance bal_;

public:
    vertex(int val) noexcept
        : val_(val),
          left_(nullptr),
          right_(nullptr),
          bal_(balance::balanced)
    {
    }
};

class avl_tree
{
    vertex *root_;

public:
    avl_tree() noexcept
        : root_(nullptr)
    {
    }

    /**
     * Returns the new root.
     */
    vertex *rot_left_once(vertex *root)
    {
        /*
        This:
           A
             C
            F G

        Becomes:
           C
         A   G
          F
        */

        vertex *new_root = root->right_;
        vertex *tmp = new_root->left_;
        new_root->left_ = root;
        new_root->left_->right_ = tmp;

        return new_root;
    }

    /**
     * Returns the new root.
     */
    vertex *rot_right_once(vertex *root)
    {
        /*
        This:
           A
         B
        D E

        Becomes:
           B
         D   A
            E
        */

        vertex *new_root = root->left_;
        vertex *tmp = new_root->right_;
        new_root->right_ = root;
        new_root->right_->left_ = tmp;

        return new_root;
    }

    /**
     * Returns the new root.
     */
    vertex *rot_left_twice(vertex *root)
    {
        /*
        This:
           A
         B   C
            F

        Becomes:
           A
         B   F
              C

        Becomes:
           F
         A   C
        B
        */

        root->right_ = rot_right_once(root->right_);
        root = rot_left_once(root);

        return root;
    }

    /**
     * Returns the new root.
     */
    vertex *rot_right_twice(vertex *root)
    {
        /*
        This:
           A
         B   C
          E

        Becomes:
           A
         E   C
        B

        Becomes:
           E
         B   A
              C
        */

        root->left_ = rot_left_once(root->left_);
        root = rot_right_once(root);

        return root;
    }

    vertex *rot_left(vertex *root)
    {
        }

    void _add(int val, vertex *curr)
    {
        // Follow tree
        // Reach destination
        // Add vertex with value and link to it
        // Go back up the tree
        // Update every balance along the way
        // Check the properties of the vertex and rotate if necessary
    }

    void add(int val)
    {
    }
};

int main()
{
    std::cout << "Hello!\n";
    return 0;
}