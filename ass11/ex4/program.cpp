#include <iostream>
#include <vector>
#include <array>
#include <queue>

/**
 * Complexity: Θ(|V|^2)
 */
std::vector<size_t> get_pos_degs(const std::vector<bool> &adj_mat, size_t verts_amt)
{
    const size_t adj_mat_size = verts_amt * verts_amt;

    std::vector<size_t> pos_degs(verts_amt, 0);

    for (size_t base_idx = 0; base_idx < adj_mat_size; base_idx += verts_amt)
    {
        for (size_t vert = 0; vert < verts_amt; ++vert)
        {
            const size_t vert_idx = base_idx + vert;
            if (adj_mat[vert_idx] == 1)
                ++pos_degs[vert];
        }
    }

    return pos_degs;
}

/**
 * Complexity: Θ(|V|^2)
 */
std::vector<size_t> get_topological_order(const std::vector<bool> &adj_mat, size_t verts_amt)
{
    std::vector<size_t> pos_degs = get_pos_degs(adj_mat, verts_amt);

    std::queue<size_t> queue;
    for (size_t vert = 0; vert < verts_amt; ++vert)
    {
        if (pos_degs[vert] == 0)
            queue.push(vert);
    }

    std::vector<size_t> order;
    order.reserve(verts_amt);

    // By the same logic of BFS,
    while (!queue.empty())
    {
        const size_t curr_vert = queue.front();
        queue.pop();

        // Update positive degrees of processed vertex.
        const size_t base_idx = curr_vert * verts_amt;
        for (size_t dst_vert = 0; dst_vert < verts_amt; ++dst_vert)
        {
            const size_t dst_idx = base_idx + dst_vert;
            if (adj_mat[dst_idx])
            {
                const size_t remaining_incoming = --pos_degs[dst_vert];
                if (!remaining_incoming)
                    queue.push(dst_vert);
            }
        }

        order.push_back(curr_vert);
    }

    if (order.size() < verts_amt)
        throw std::runtime_error("tried to sort cyclic graph topologically");

    return order;
}

int main()
{
    constexpr size_t adj_mat_verts_amt = 3;

    /* clang-format off */
    std::vector<bool> adj_mat = 
    {
        0, 1, 1,
        0, 0, 1,
        0, 0, 0,
    };
    /* clang-format on */

    const std::vector<size_t> order = get_topological_order(adj_mat, adj_mat_verts_amt);
    for (size_t i = 0; i < adj_mat_verts_amt; ++i)
        std::cout << "[" << i << "] " << order[i] << "\n";

    return 0;
}