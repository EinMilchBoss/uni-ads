#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <array>
#include <unordered_map>

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

const std::string instruction_mnemonic_strings[] = {
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

static const std::unordered_map<std::string, instruction_mnemonic> instruction_mnemonic_translation = {
    {
        "ADD",
        instruction_mnemonic::ADD,
    },
    {
        "SUB",
        instruction_mnemonic::SUB,
    },
    {
        "MUL",
        instruction_mnemonic::MUL,
    },
    {
        "DIV",
        instruction_mnemonic::DIV,
    },
    {
        "LDA",
        instruction_mnemonic::LDA,
    },
    {
        "LDK",
        instruction_mnemonic::LDK,
    },
    {
        "STA",
        instruction_mnemonic::STA,
    },
    {
        "INP",
        instruction_mnemonic::INP,
    },
    {
        "OUT",
        instruction_mnemonic::OUT,
    },
    {
        "HLT",
        instruction_mnemonic::HLT,
    },
    {
        "JMP",
        instruction_mnemonic::JMP,
    },
    {
        "JEZ",
        instruction_mnemonic::JEZ,
    },
    {
        "JNE",
        instruction_mnemonic::JNE,
    },
    {
        "JLZ",
        instruction_mnemonic::JLZ,
    },
    {
        "JLE",
        instruction_mnemonic::JLE,
    },
    {
        "JGZ",
        instruction_mnemonic::JGZ,
    },
    {
        "JGE",
        instruction_mnemonic::JGE,
    },
};

struct instruction
{
    uint32_t number;
    instruction_mnemonic mnemonic;
    uint32_t argument;

    void print() const noexcept
    {
        std::cout << number << ' ' << instruction_mnemonic_strings[static_cast<int>(mnemonic)] << ' ' << argument << '\n';
    }
};

class program
{
    size_t program_counter_ = 0;
    std::array<int32_t, 8> data_{};

    const std::vector<instruction> instructions_;

public:
    program(std::vector<instruction> &&instructions) : instructions_{std::move(instructions)}
    {
    }

    void run()
    {
        bool is_running = true;
        while (is_running && program_counter_ < instructions_.size())
        {
            const instruction &current = instructions_[program_counter_];
            current.print();
            print_dump();

            program_counter_ += 1;

            switch (current.mnemonic)
            {
            case instruction_mnemonic::ADD:
                data_[0] += data_[current.argument];
                break;
            case instruction_mnemonic::SUB:
                data_[0] -= data_[current.argument];
                break;
            case instruction_mnemonic::MUL:
                data_[0] *= data_[current.argument];
                break;
            case instruction_mnemonic::DIV:
                data_[0] /= data_[current.argument];
                break;
            case instruction_mnemonic::LDA:
                data_[0] = data_[current.argument];
                break;
            case instruction_mnemonic::LDK:
                data_[0] = current.argument;
                break;
            case instruction_mnemonic::STA:
                data_[current.argument] = data_[0];
                break;
            case instruction_mnemonic::INP:
            case instruction_mnemonic::OUT:
                throw std::runtime_error("INP and OUT are not defined.");
                break;
            case instruction_mnemonic::HLT:
                print_dump();
                is_running = false;
                break;
            case instruction_mnemonic::JMP:
                program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JEZ:
                if (data_[0] == 0)
                    program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JNE:
                if (data_[0] != 0)
                    program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JLZ:
                if (data_[0] < 0)
                    program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JLE:
                if (data_[0] <= 0)
                    program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JGZ:
                if (data_[0] > 0)
                    program_counter_ = current.argument - 1;
                break;
            case instruction_mnemonic::JGE:
                if (data_[0] >= 0)
                    program_counter_ = current.argument - 1;
                break;
            default:
                throw std::runtime_error("Only known mnemonics shall be used.");
            }
        }
    }

    void print_instructions() const noexcept
    {
        for (const instruction &i : instructions_)
        {
            std::cout << instruction_mnemonic_strings[static_cast<int>(i.mnemonic)] << "\n";
        }
    }

    void print_dump() const noexcept
    {
        std::cout << "DATA:\n";
        for (size_t i = 0; i < data_.size(); i++)
        {
            std::cout << "[" << std::setfill('0') << std::setw(2) << i << "] " << data_[i] << "\n";
        }
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
    const instruction_mnemonic mnemonic = instruction_mnemonic_translation.at(line_tokens[1].c_str());
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
    // std::array<int32_t, 32> data_memory{};
    // std::vector<std::string> program_memory{};

    std::vector<instruction> instructions = read_instructions("./instructions.txt");
    program p{std::move(instructions)};
    p.print_instructions();
    p.run();

    return 0;
}