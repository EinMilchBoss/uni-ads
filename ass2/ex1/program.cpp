#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <array>

enum class instruction_mnemonic
{
    ADD,
    SUB,
    MUL,
    DIV,
    LDA,
    LDK,
    STA,
    INP,
    OUT,
    HLT,
    JMP,
    JEZ,
    JNE,
    JLZ,
    JLE,
    JGZ,
    JGE,
};

constexpr size_t instruction_mnemonic_name_size = 17;
constexpr const char *const instruction_mnemonic_name[instruction_mnemonic_name_size] = {
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "LDA",
    "LDK",
    "STA",
    "INP",
    "OUT",
    "HLT",
    "JMP",
    "JEZ",
    "JNE",
    "JLZ",
    "JLE",
    "JGZ",
    "JGE",
};

instruction_mnemonic parse_instruction_mnemonic(const std::string &s)
{
    for (size_t i = 0; i < instruction_mnemonic_name_size; i++)
    {
        if (s == instruction_mnemonic_name[i])
        {
            return static_cast<instruction_mnemonic>(i);
        }
    }
    throw std::runtime_error("The user shall only use valid instruction mnemonics.");
}

struct instruction
{
    uint32_t number;
    instruction_mnemonic mnemonic;
    uint32_t argument;

    void print() const noexcept
    {
        std::cout << number << ' ' << instruction_mnemonic_name[static_cast<int>(mnemonic)] << ' ' << argument << '\n';
    }
};

class program
{
    const std::vector<instruction> instructions_;

    void print_dump(std::array<int32_t, 8> data) const noexcept
    {
        std::cout << "DATA:\n";
        for (size_t i = 0; i < data.size(); i++)
        {
            std::cout << "[" << std::setfill('0') << std::setw(2) << i << "] " << data[i] << "\n";
        }
    }

public:
    program(std::vector<instruction> &&instructions) : instructions_{std::move(instructions)}
    {
    }

    void run()
    {
        bool is_running = true;
        size_t program_counter = 0;
        std::array<int32_t, 8> data{};

        while (is_running && program_counter < instructions_.size())
        {
            // Fetch instruction.
            const instruction &current = instructions_[program_counter];
            program_counter += 1;

            // Decode + Execute instruction.
            switch (current.mnemonic)
            {
            case instruction_mnemonic::ADD:
                data[0] += data[current.argument];
                break;
            case instruction_mnemonic::SUB:
                data[0] -= data[current.argument];
                break;
            case instruction_mnemonic::MUL:
                data[0] *= data[current.argument];
                break;
            case instruction_mnemonic::DIV:
                data[0] /= data[current.argument];
                break;
            case instruction_mnemonic::LDA:
                data[0] = data[current.argument];
                break;
            case instruction_mnemonic::LDK:
                data[0] = current.argument;
                break;
            case instruction_mnemonic::STA:
                data[current.argument] = data[0];
                break;
            case instruction_mnemonic::INP:
            case instruction_mnemonic::OUT:
                throw std::runtime_error("INP and OUT are not defined.");
                break;
            case instruction_mnemonic::HLT:
                is_running = false;
                break;
            case instruction_mnemonic::JMP:
                program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JEZ:
                if (data[0] == 0)
                    program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JNE:
                if (data[0] != 0)
                    program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JLZ:
                if (data[0] < 0)
                    program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JLE:
                if (data[0] <= 0)
                    program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JGZ:
                if (data[0] > 0)
                    program_counter = current.argument - 1;
                break;
            case instruction_mnemonic::JGE:
                if (data[0] >= 0)
                    program_counter = current.argument - 1;
                break;
            default:
                throw std::runtime_error("Only known mnemonics shall be used.");
            }
        }

        // Dump memory to console.
        print_dump(data);
    }

    void print_instructions() const noexcept
    {
        std::cout << "INSTRUCTIONS:\n";
        for (const instruction &i : instructions_)
            i.print();
    }
};

std::vector<std::string> split_by_whitespace(const std::string &s)
{
    std::stringstream stream{s};

    std::vector<std::string> tokens;

    std::string current_token;
    while (stream >> current_token)
        tokens.push_back(current_token);

    return tokens;
}

uint32_t parse_u32(const std::string &s)
{
    try
    {
        return static_cast<uint32_t>(std::stoul(s));
    }
    catch (const std::exception &_)
    {
        throw std::runtime_error("Input could not be parsed to a 32bit unsigned integer.");
    }
}

instruction parse_instruction(const std::vector<std::string> &line_tokens)
{

    if (line_tokens.size() != 3)
        throw std::runtime_error("Every line shall consist of 3 tokens.");

    const uint32_t number = parse_u32(line_tokens[0]);
    const instruction_mnemonic mnemonic = parse_instruction_mnemonic(line_tokens[1]);
    const uint32_t argument = parse_u32(line_tokens[2]);

    return {
        number,
        mnemonic,
        argument,
    };
}

bool are_instruction_numbers_sequential(const std::vector<instruction> &instructions)
{
    for (size_t i = 0; i < instructions.size(); i++)
    {
        if (instructions[i].number != i + 1)
            return false;
    }
    return true;
}

bool is_comment_line(const std::string &line)
{
    return line.find("//") != std::string::npos;
}

bool is_empty_line(const std::string &line)
{
    return line.find_first_not_of(' ') == std::string::npos;
}

std::vector<instruction> read_instructions(const char *file)
{
    std::ifstream ifs{file};

    std::vector<instruction> instructions;
    std::string line;
    while (std::getline(ifs, line))
    {
        if (is_empty_line(line) || is_comment_line(line))
            continue;

        const std::vector<std::string> tokens = split_by_whitespace(line);

        instructions.push_back(parse_instruction(tokens));
    }

    if (!are_instruction_numbers_sequential(instructions))
        throw std::runtime_error("The instruction numbers shall be sequential.");

    return instructions;
}

int main()
{
    std::vector<instruction> instructions = read_instructions("./instructions.txt");
    program p{std::move(instructions)};
    p.print_instructions();
    p.run();

    return 0;
}