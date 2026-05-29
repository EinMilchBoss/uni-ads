#include <iostream>
#include <vector>
#include <queue>
#include <array>

std::vector<size_t> bfs(const std::vector<bool> &adj_mat, size_t vert_amt, size_t start_vert)
{
    std::queue<size_t> queue;
    queue.push(start_vert);

    std::vector<bool> visited(vert_amt, false);
    visited[start_vert] = true;

    std::vector<size_t> proc_order(vert_amt, 0);
    size_t iteration = 1;

    while (!queue.empty())
    {
        size_t src_vert = queue.front();
        queue.pop();

        const size_t base_idx = src_vert * vert_amt;
        for (size_t dst_vert = 0; dst_vert < vert_amt; ++dst_vert)
        {
            auto visited_bit = visited[dst_vert];

            const size_t dst_idx = base_idx + dst_vert;
            if (adj_mat[dst_idx] && !static_cast<bool>(visited_bit))
            {
                queue.push(dst_vert);
                visited_bit.flip();
            }
        }

        proc_order[src_vert] = iteration++;
    }

    return proc_order;
}

void do_dfs(
    const std::vector<bool> &adj_mat,
    size_t vert_amt,
    size_t src_vert,
    std::vector<bool> &visited,
    std::vector<size_t> &proc_order,
    size_t &iteration)
{
    proc_order[src_vert] = iteration++;

    const size_t base_idx = src_vert * vert_amt;
    for (size_t dst_vert = 0; dst_vert < vert_amt; ++dst_vert)
    {
        auto visited_bit = visited[dst_vert];

        const size_t dst_idx = base_idx + dst_vert;
        if (adj_mat[dst_idx] && !static_cast<bool>(visited_bit))
        {
            visited_bit.flip();
            do_dfs(adj_mat, vert_amt, dst_vert, visited, proc_order, iteration);
        }
    }
}

std::vector<size_t> dfs(const std::vector<bool> &adj_mat, size_t vert_amt, size_t start_vert)
{
    std::vector<bool> visited(vert_amt, false);
    visited[start_vert] = true;

    std::vector<size_t> proc_order(vert_amt, 0);
    size_t iteration = 1;

    do_dfs(adj_mat, vert_amt, start_vert, visited, proc_order, iteration);

    return proc_order;
}

int main()
{
    constexpr size_t adj_mat_vert_amt = 9;

    /* clang-format off */
    std::vector<bool> adj_mat = 
    {
        0, 1, 1, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0,
        1, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 1, 0, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 1, 0, 0, 0,
    };
    /* clang-format on */

    const auto proc_order_bfs = bfs(adj_mat, adj_mat_vert_amt, 0);
    std::cout << "BFS:\n";
    for (size_t i = 0; i < proc_order_bfs.size(); ++i)
        std::cout << "[" << i << "] " << proc_order_bfs[i] << "\n";

    const auto proc_order_dfs = dfs(adj_mat, adj_mat_vert_amt, 0);
    std::cout << "DFS:\n";
    for (size_t i = 0; i < proc_order_dfs.size(); ++i)
        std::cout << "[" << i << "] " << proc_order_dfs[i] << "\n";

    return 0;
}