#include <iostream>
#include <vector>
#include <queue>

struct vertex
{
    std::vector<size_t> succ_vert_idxs;
};

/**
 * Complexity: Θ(|V| + |E|)
 */
std::vector<size_t> get_pos_degs(const std::vector<vertex> &verts)
{
    std::vector<size_t> pos_degs(verts.size(), 0);

    for (const vertex &vert : verts)
    {
        for (size_t succ_vert_idx : vert.succ_vert_idxs)
            ++pos_degs[succ_vert_idx];
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

    // Apply BFS for all verticies v where deg+(v) = 0.
    // Complexity: Θ(|V| + |E|)
    while (!queue.empty())
    {
        const size_t curr_vert_idx = queue.front();
        queue.pop();

        // Update positive degrees of successor verticies.
        for (size_t succ_vert_idx : verts[curr_vert_idx].succ_vert_idxs)
        {
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
    constexpr size_t verts_size = 3;

    std::vector<vertex> verts(verts_size, vertex{});
    verts[0].succ_vert_idxs.push_back(1);
    verts[0].succ_vert_idxs.push_back(2);
    verts[1].succ_vert_idxs.push_back(2);

    const std::vector<size_t> order = get_topological_order(verts);
    for (size_t i = 0; i < verts_size; ++i)
        std::cout << "[" << i << "] " << order[i] << "\n";

    return 0;
}