#include <stdexcept>

#include "avl.hpp"

using vertex = avl_tree::vertex;

avl_tree::vertex::vertex(int val) noexcept
    : val_(val),
      left_(nullptr),
      right_(nullptr),
      bal_(0)
{
}

avl_tree::avl_tree() noexcept
    : root_(nullptr)
{
}

/**
 * Returns the new root.
 */
vertex *avl_tree::rot_left_once(vertex *root)
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
vertex *avl_tree::rot_right_once(vertex *root)
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
vertex *avl_tree::rot_left_twice(vertex *root)
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
vertex *avl_tree::rot_right_twice(vertex *root)
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

vertex *avl_tree::rot_left(vertex *root)
{

    return nullptr;
}

void avl_tree::_add(const int val, vertex *curr)
{
    // Follow tree
    // Reach destination
    // Add vertex with value and link to it
    // Go back up the tree
    // Update every balance along the way
    // Check the properties of the vertex and rotate if necessary

    // We reached our destination
    if (curr == nullptr)
    {
    }

    // Follow through the tree branches until we reach our destination.
    if (val < curr->val_ && curr->left_ != nullptr)
    {
        _add(val, curr->left_);
    }
    else if (curr->val_ < val && curr->right_ != nullptr)
    {
        _add(val, curr->right_);
    }
    else
    {
        throw std::runtime_error("added same value twice to avl tree");
    }
}

void avl_tree::add(int val)
{
}