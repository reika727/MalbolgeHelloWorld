/**
 * @file malbolge.hpp
 * @brief Malbolge 言語のプロパティ群
 */

#ifndef MALBOLGE_HPP
#define MALBOLGE_HPP
#include <optional>
#include <cstdint>

namespace malbolge {
    /**
     * @brief Malbolge における 1 ワード
     * @note Malbolge における 1 ワードは符号なし 10 トリット整数であり、
     * @note 0 から 3^10-1 (=59048) まで表せるので、符号なし 16 ビット整数を用いる。
     */
    using word = std::uint16_t;

    //! 1 ワードが表せる数の幅
    static inline constexpr word word_size = 59049;

    /**
     * @brief 命令の一覧
     */
    enum class Instruction : word {
        MovD = 'j',
        Jmp = 'i',
        RotR = '*',
        Op = 'p',
        Out = '<',
        In = '/',
        Exit = 'v',
        Nop = 'o'
    };

    //! 命令の一覧（イテレーション用）
    static inline constexpr Instruction instructions[] = {
        Instruction::MovD,
        Instruction::Jmp,
        Instruction::RotR,
        Instruction::Op,
        Instruction::Out,
        Instruction::In,
        Instruction::Exit,
        Instruction::Nop
    };

    /**
     * @brief 右に一けた三進巡回シフトする
     */
    word trit_rotate_right(const word t);

    /**
     * @brief op 演算（もしくは crazy 演算）
     */
    word op(word t1, word t2);

    /**
     * @brief メモリから命令をデコードする
     * @param address アドレス
     * @param data address に格納されているワード
     * @param treats_non_opcode_as_nop デコード結果が Instruction のどれでもない場合 Nop とみなすか
     * @return data が空白文字以外の ASCII 印字可能文字でない場合は std::nullopt。
     * @return data のデコード結果が Instruction に一致した場合はデコード結果。
     * @return 一致しなかった場合、treats_non_opcode_as_nop == true ならば Instruction::Nop。
     * @return treats_non_opcode_as_nop == false ならば std::nullopt。
     */
    std::optional<Instruction> decode_operation(const word address, const word data, const bool treats_non_opcode_as_nop = true);

    /**
     * @brief デコード後に instruction に戻るワードを求める
     * @param address 格納先のアドレス
     * @param instruction 命令
     * @return デコード後に instruction に戻るワード
     */
    word encode_instruction(const word address, const Instruction instruction);

    /**
     * @brief メモリを暗号化する
     * @param data 暗号化するワード
     * @return data が空白文字以外の ASCII 印字可能文字である場合は暗号化結果。
     * @return そうでない場合は std::nullopt
     */
    std::optional<word> encrypt_code(const word data);
}
#endif
