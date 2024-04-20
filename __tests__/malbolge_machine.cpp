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
            if (!malbolge::decode_operation(itr - std::begin(memory), x, false)) {
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
    const auto opcode = malbolge::decode_operation(c, memory[c]);
    if (!opcode) {
        return false;
    }
    switch (*opcode) {
        case malbolge::Instruction::MovD:
            d = memory[d];
            break;
        case malbolge::Instruction::Jmp:
            c = memory[d];
            break;
        case malbolge::Instruction::RotR:
            a = memory[d] = malbolge::trit_rotate_right(memory[d]);
            break;
        case malbolge::Instruction::Op:
            a = memory[d] = malbolge::op(a, memory[d]);
            break;
        case malbolge::Instruction::Out:
            os.put(static_cast<unsigned char>(a));
            break;
        case malbolge::Instruction::In:
            {
                const auto x = is.get();
                a = (is.eof() ? 59048 : x);
            }
            break;
        case malbolge::Instruction::Exit:
            return true;
        case malbolge::Instruction::Nop:
            ;
            break;
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
