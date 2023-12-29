#include "malbolge_machine.hpp"
#include <iterator>
#include <stdexcept>

/**
 * @copydoc malbolge_machine::malbolge_machine(std::istream &)
 */
malbolge_machine::malbolge_machine(std::istream &is)
{
    auto itr = std::begin(memory);
    for (unsigned char x; is >> std::skipws >> x; *itr++ = x) {
        if (33 <= x && x < 127) {
            if (!malbolge::decode_operation(std::distance(std::begin(memory), itr), x, false)) {
                throw std::runtime_error("invalid character in source file");
            }
        }
        if (itr == std::end(memory)) {
            throw std::runtime_error("input file too long");
        }
    }
    for (; itr != std::end(memory); ++itr) {
        *itr = malbolge::op(*(itr - 1), *(itr - 2));
    }
}

/**
 * @copydoc malbolge_machine::malbolge_machine(std::istream &is, std::ostream &os)
 */
bool malbolge_machine::exec_one_step(std::istream &is, std::ostream &os)
{
    if (const auto opcode = malbolge::decode_operation(c, memory[c]); !opcode) {
        return false;
    } else if (opcode == malbolge::Instruction::MovD) {
        d = memory[d];
    } else if (opcode == malbolge::Instruction::Jmp) {
        c = memory[d];
    } else if (opcode == malbolge::Instruction::RotR) {
        a = memory[d] = malbolge::trit_rotate_right(memory[d]);
    } else if (opcode == malbolge::Instruction::Op) {
        a = memory[d] = malbolge::op(a, memory[d]);
    } else if (opcode == malbolge::Instruction::Out) {
        os << static_cast<unsigned char>(a);
    } else if (opcode == malbolge::Instruction::In) {
        const auto x = is.get();
        a = (is.eof() ? 59048 : x);
    } else if (opcode == malbolge::Instruction::Exit) {
        return true;
    }
    if (const auto encrypted = malbolge::encrypt_code(memory[c])) {
        memory[c] = *encrypted;
    } else {
        throw std::runtime_error("memory[c] is not a graphical ASCII");
    }
    c = (c + 1) % malbolge::word_size;
    d = (d + 1) % malbolge::word_size;
    return false;
}
