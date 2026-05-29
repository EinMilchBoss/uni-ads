#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

#include <cassert>

/**
 * Returns `true` if a cylce is found, else `false`.
 * The function short-circuits all other paths if it found the first cycle.
 */
bool do_find_first_cycle(
    const std::vector<bool> &adj_mat,
    size_t vert_amt,
    size_t start_vert,
    size_t curr_vert,
    std::vector<bool> &visited,
    std::vector<size_t> &cycle_verts)
{
    const size_t base_idx = curr_vert * vert_amt;
    for (size_t next_vert = 0; next_vert < vert_amt; ++next_vert)
    {
        const size_t next_idx = base_idx + next_vert;

        if (!adj_mat[next_idx])
            continue;

        // A cycle was found. Start backtracking.
        if (next_vert == start_vert)
        {
            cycle_verts.push_back(curr_vert);
            return true;
        }

        auto visited_bit = visited[next_vert];
        if (!visited_bit)
        {
            visited_bit.flip();

            // In case a cycle was found, skip the other options.
            bool found_cycle = do_find_first_cycle(adj_mat, vert_amt, start_vert, next_vert, visited, cycle_verts);
            if (found_cycle)
            {
                cycle_verts.push_back(curr_vert);
                return true;
            }
        }
    }

    // We didn't find a cycle after trying everything.
    return false;
}

std::optional<std::vector<size_t>> find_first_cycle(const std::vector<bool> &adj_mat, size_t vert_amt)
{
    std::vector<size_t> cycle_verts;
    cycle_verts.reserve(vert_amt);

    std::vector<bool> visited;
    visited.reserve(vert_amt);

    for (size_t start_vert = 0; start_vert < vert_amt; ++start_vert)
    {
        // Prepare for recursion.
        std::fill(visited.begin(), visited.end(), false);
        visited[start_vert] = true;

        assert(cycle_verts.size() == 0 && "no cycle has been found so no cycle vertecies were added yet");
        bool found_cycle = do_find_first_cycle(adj_mat, vert_amt, start_vert, start_vert, visited, cycle_verts);
        if (found_cycle)
        {
            std::reverse(cycle_verts.begin(), cycle_verts.end());
            return cycle_verts;
        }
    }

    return std::nullopt;
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

    if (const auto first_cycle = find_first_cycle(adj_mat, adj_mat_vert_amt))
    {
        std::cout << "Found cycle!\n";
        std::cout << "First one:\n";
        for (size_t i = 0; i < first_cycle->size(); ++i)
            std::cout << "[" << i << "] " << first_cycle->at(i) << "\n";
    }
    else
    {
        std::cout << "Did not find a cycle.\n";
    }

    return 0;
}