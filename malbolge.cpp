/**
 * @file malbolge.cpp
 * @see malbolge.hpp
 */

#include "malbolge.hpp"
#include <string_view>
#include <utility>

namespace malbolge {
    /**
     * @copydoc malbolge::trit_rotate_right(const word)
     */
    word trit_rotate_right(const word t)
    {
        return t / 3 + t % 3 * (word_size / 3);
    }

    /**
     * @copydoc malbolge::op(word, word)
     */
    word op(word t1, word t2)
    {
        constexpr word table[3][3] = {
            {1, 0, 0},
            {1, 0, 2},
            {2, 2, 1}
        };
        word pow = 1, result = 0;
        for (unsigned i = 0; i < 10; ++i) {
            result += table[std::exchange(t2, t2 / 3) % 3][std::exchange(t1, t1 / 3) % 3] * std::exchange(pow, pow * 3);
        }
        return result;
    }

    /**
     * @copydoc malbolge::decode_operation(const word, const word, const bool)
     */
    std::optional<Instruction> decode_operation(const word address, const word data, const bool treats_non_opcode_as_nop)
    {
        constexpr std::string_view opcode_decode_table =
            R"(+b(29e*j1VMEKLyC})8&m#~W>qxdRp0wkrUo[D7,XTcA"lI.v%{gJh4G\-=O@5`_3i<?Z';FNQuY]szf$!BS/|t:Pn6^Ha)";
        if (data < 33 || data > 126) {
            return std::nullopt;
        }
        for (const auto instruction : instructions) {
            if (opcode_decode_table[(data - 33 + address) % opcode_decode_table.length()] == std::to_underlying(instruction)) {
                return instruction;
            }
        }
        if (treats_non_opcode_as_nop) {
            return Instruction::Nop;
        } else {
            return std::nullopt;
        }
    }

    /**
     * @copydoc malbolge::encode_instruction(const word, const Instruction)
     */
    word encode_instruction(const word address, const Instruction instruction)
    {
        word tmp =
              instruction == Instruction::MovD ? '('
            : instruction == Instruction::Jmp ? 'b'
            : instruction == Instruction::RotR ? '\''
            : instruction == Instruction::Op ? '>'
            : instruction == Instruction::Out ? 'c'
            : instruction == Instruction::In ? 'u'
            : instruction == Instruction::Exit ? 'Q'
            : /* instruction == Instruction::Nop */ 'D';
        tmp = (tmp + 94 - address % 94) % 94;
        return tmp < 33 ? tmp + 94 : tmp;
    }

    /**
     * @copydoc malbolge::encrypt_code(const word)
     */
    std::optional<word> encrypt_code(const word data)
    {
        constexpr std::string_view code_encryption_table =
            R"(5z]&gqtyfr$(we4{WP)H-Zn,[%\3dL+Q;>U!pJS72FhOA1CB6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G"i@)";
        if (33 <= data && data < 127) {
            return code_encryption_table[data - 33];
        } else {
            return std::nullopt;
        }
    }
}
