#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <cstring>

/**
 * All uppercase letters and space.
 */
constexpr size_t alphabet_max = 26 + 1;

size_t get_char_index(char c)
{
    switch (c)
    {
    case ' ':
        return 26;
    default:
        return static_cast<size_t>(c - 'A');
    }

    throw std::runtime_error("tried to get the index of an invalid character");
}

int32_t naive_search(const char *txt, const char *pattern)
{
    int32_t matches_amount = 0;

    for (const char *o = txt; *o != '\0'; ++o)
    {
        bool matches = true;

        size_t offset = 0;
        for (const char *i = pattern; *i != '\0'; ++i)
        {
            if (o[offset++] != *i)
            {
                matches = false;
                break;
            }
        }

        if (matches)
            ++matches_amount;
    }

    return matches_amount;
}

int32_t boyer_moore_search(const char *txt, size_t txt_len, const char *pat, size_t pat_len)
{
    // Generate shift table.
    size_t shifts[alphabet_max];
    for (size_t i = 0; i < alphabet_max; ++i)
        shifts[i] = pat_len;
    for (size_t i = 0; i < pat_len; ++i)
        shifts[get_char_index(pat[i])] = pat_len - i - 1;

    int32_t matches_amount = 0;
    size_t txt_idx, pat_idx;
    txt_idx = pat_idx = pat_len - 1;
    while (txt_idx < txt_len)
    {
        if (txt[txt_idx] == pat[pat_idx])
        {
            if (pat_idx == 0)
            {
                ++matches_amount;

                txt_idx += pat_len;
                pat_idx = pat_len - 1;
            }
            else
            {
                --txt_idx;
                --pat_idx;
            }
        }
        else
        {
            size_t shift = shifts[get_char_index(txt[txt_idx])];

            size_t current_number = pat_len - pat_idx;
            if (current_number > shift)
                txt_idx += current_number;
            else
                txt_idx += shift;
            pat_idx = pat_len - 1;
        }
    }

    return matches_amount;
}

void get_alphabet(const char *text, char *out_alph)
{
    // Add all chars.
    for (const char *c = text; *c != '\0'; ++c)
    {
        out_alph[get_char_index(*c)] = *c;
    }

    // Collapse chars into sequence without holes
    size_t back = alphabet_max - 1;
    for (size_t front = 0; front < alphabet_max; ++front)
    {
        if (out_alph[front])
            continue;

        // Get next char from the back.
        char fill = 0;
        for (; back > front; --back)
        {
            if (!out_alph[back])
                continue;

            fill = out_alph[back];
            out_alph[back] = 0;
            break;
        }

        // If no fill char was found, i and j overlap.
        // We are done.
        if (!fill)
            break;

        out_alph[front] = fill;
    }
}

int main()
{
    const char *txt = "ALGORITHMEN UND DATENSTRUKTUREN";
    const char *pattern = "DATEN";
    char alph[alphabet_max] = {0};

    get_alphabet(txt, alph);

    for (size_t i = 0; i < alphabet_max && alph[i] != '\0'; ++i)
        std::cout << alph[i];
    std::cout << std::endl;

    std::cout << "Naive: " << naive_search(txt, pattern) << "\n";
    std::cout << "Boyer-Moore: " << boyer_moore_search(txt, strlen(txt), pattern, strlen(pattern)) << "\n";

    return 0;
}