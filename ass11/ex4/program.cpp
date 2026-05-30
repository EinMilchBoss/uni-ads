#include <iostream>
#include <vector>
#include <array>
#include <queue>

struct vertex
{
    size_t idx;
    std::vector<vertex *> succs;
};

/**
 * Complexity: Θ(|V| + |E|)
 */
std::vector<size_t> get_pos_degs(const std::vector<vertex> &verts)
{
    std::vector<size_t> pos_degs(verts.size(), 0);

    for (const vertex &vert : verts)
    {
        for (size_t i = 0; i < vert.succs.size(); ++i)
            ++pos_degs[vert.succs[i]->idx];
    }

    return pos_degs;
}

/**
 * Complexity: Θ(|V| + |E|)
 */
std::vector<size_t> get_topological_order(const std::vector<vertex> &verts)
{
    // Complexity: Θ(|V| + |E|)
    std::vector<size_t> pos_degs = get_pos_degs(verts);

    // Complexity: Θ(|V|)
    std::queue<size_t> queue;
    for (size_t i = 0; i < pos_degs.size(); ++i)
    {
        if (pos_degs[i] == 0)
            queue.push(i);
    }

    std::vector<size_t> order;
    order.reserve(verts.size());

    // Apply BFS for all vertecies v where deg+(v) = 0.
    // Complexity: Θ(|V| + |E|)
    while (!queue.empty())
    {
        const size_t curr_vert_idx = queue.front();
        queue.pop();

        // Update positive degrees of successor vertecies.
        for (size_t succ_idx = 0; succ_idx < verts[curr_vert_idx].succs.size(); ++succ_idx)
        {
            const size_t succ_vert_idx = verts[curr_vert_idx].succs[succ_idx]->idx;

            --pos_degs[succ_vert_idx];
            if (pos_degs[succ_vert_idx] == 0)
                queue.push(succ_vert_idx);
        }

        order.push_back(curr_vert_idx);
    }

    if (order.size() < verts.size())
        throw std::runtime_error("tried to sort cyclic graph topologically");

    return order;
}

int main()
{
    constexpr size_t adj_mat_verts_amt = 3;

    std::vector<vertex> verts(adj_mat_verts_amt, vertex{});
    for (size_t i = 0; i < adj_mat_verts_amt; ++i)
        verts[i].idx = i;
    verts[0].succs.push_back(&verts[1]);
    verts[0].succs.push_back(&verts[2]);
    verts[1].succs.push_back(&verts[2]);

    const std::vector<size_t> order = get_topological_order(verts);
    for (size_t i = 0; i < adj_mat_verts_amt; ++i)
        std::cout << "[" << i << "] " << order[i] << "\n";

    return 0;
}