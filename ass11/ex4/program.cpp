#include <iostream>
#include <vector>
#include <array>

std::vector<int16_t> get_pos_degs(const std::vector<bool> &adj_mat, size_t verts_amt)
{
    const size_t adj_mat_size = verts_amt * verts_amt;

    std::vector<int16_t> pos_degs(verts_amt, 0);

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

std::vector<size_t> get_topological_order(const std::vector<bool> &adj_mat, size_t verts_amt)
{
    constexpr int16_t already_sorted = -1;
    std::vector<int16_t> pos_degs = get_pos_degs(adj_mat, verts_amt);

    std::vector<size_t> order;
    order.reserve(verts_amt);

    size_t remaining_verts_amt = verts_amt;
    size_t src_vert = 0;
    size_t successive_skips = 0;
    while (remaining_verts_amt > 0)
    {
        if (successive_skips == remaining_verts_amt)
            throw std::runtime_error("tried to sort cyclic graph topologically");

        switch (pos_degs[src_vert])
        {
        case already_sorted:
            break;
        case 0:
        {
            // Remove outgoing edges of that vertex and update positive degrees.
            const size_t base_idx = src_vert * verts_amt;
            for (size_t dst_vert = 0; dst_vert < verts_amt; ++dst_vert)
            {
                const size_t dst_idx = base_idx + dst_vert;
                if (adj_mat[dst_idx] == 1)
                    --pos_degs[dst_vert];
            }

            pos_degs[src_vert] = already_sorted;
            order.push_back(src_vert);
            --remaining_verts_amt;

            successive_skips = 0;

            break;
        }
        default:
            ++successive_skips;
            break;
        }

        src_vert = (src_vert + 1) % verts_amt;
    }

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