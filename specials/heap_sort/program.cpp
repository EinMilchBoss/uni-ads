#include <iostream>
#include <random>
#include <cassert>
#include <cstdint>
#include <cstdlib>

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void sift_down(int *arr, size_t arr_len, size_t root_idx) {
    assert(arr != nullptr);
    assert(arr_len >= 1);
    assert(root_idx < arr_len);
    
    const size_t left_idx = 2 * root_idx + 1;
    const size_t right_idx = left_idx + 1;
    
    // Exit the recursion if there are no children to swap with.
    if (left_idx >= arr_len) {
        return;
    }
    
    // => We have at least one child (the left one)!
    
    int &root = arr[root_idx];
    int &left = arr[left_idx];
    int &right = arr[right_idx];
    
    if (right_idx < arr_len) {
        // The root has 2 children.
        
        // We are only interested in the bigger child.
        if (left >= right) {
            // Swap with the left child if it's bigger than the root.
            if (left > root) {
                swap(left, root);
                sift_down(arr, arr_len, left_idx);
            }
        } else {
            // Swap with the right child if it's bigger than the root.
            if (right > root) {
                swap(right, root);
                sift_down(arr, arr_len, right_idx);
            }
        }
    } else if (left > root) {
        // The root has only one child (the left one).
        // It's also bigger than the root, so we swap.
        swap(left, root);
        sift_down(arr, arr_len, left_idx);
    }
}

void build_heap(int *arr, size_t arr_len) {
    for (size_t i = (arr_len - 2) / 2; i > 0; i--) {
        sift_down(arr, arr_len, i);
    }
    sift_down(arr, arr_len, 0);
}

void heap_sort(int *arr, size_t arr_len) {
    build_heap(arr, arr_len);
    
    for (size_t i = arr_len - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        sift_down(arr, i, 0);
    }
}

void set_array_randomly(
    int *arr, 
    size_t arr_len, 
    std::mt19937 &rng, 
    std::uniform_int_distribution<int> &dist
) {
    for (size_t j = 0; j < arr_len; j++) {
        arr[j] = dist(rng);
    }
}

void print_array(int *arr, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++) {
        std::cout << "[" << i << "] " << arr[i] << "\n";
    }
}

bool is_sorted_asc(int *arr, size_t arr_len) {
    if (arr_len == 0)
        return true;
    
    int prev = arr[0];
    for (size_t i = 1; i < arr_len; i++) {
        if (prev > arr[i])
            return false;
        prev = arr[i];
    }
    return true;
}


#define DEBUG

int main() {
    constexpr size_t test_runs = 1000;
    constexpr size_t arr_len = 1000;
    
    int arr[arr_len];
    
    std::random_device seed_generator;
    const unsigned int seed = seed_generator();
    std::mt19937 rng{seed};
    std::uniform_int_distribution<int> dist{0, 9};
    std::cout << "Random tests with seed " << seed << "...\n";
    for (size_t i = 0; i < test_runs; i++) {
        set_array_randomly(arr, arr_len, rng, dist);

#ifdef DEBUG
        std::cout << "Before:\n";
        print_array(arr, arr_len);
#endif
        
        heap_sort(arr, arr_len);
        
#ifdef DEBUG
        std::cout << "After:\n";
        print_array(arr, arr_len);
#endif
        
        if (!is_sorted_asc(arr, arr_len)) {
            std::cerr << "The sorting algorithm did not sort (asc).\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << "All tests passed successfully.\n";
    return EXIT_SUCCESS;
}
