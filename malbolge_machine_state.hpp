/**
 * @file malbolge_machine_state.hpp
 * @brief Malbolge 仮想機械の状態
 */

#ifndef MALBOLGE_MACHINE_STATE_HPP
#define MALBOLGE_MACHINE_STATE_HPP
#include "malbolge.hpp"
#include <string>
#include <map>
#include <utility>
#include <optional>
#include <memory>
#include <string_view>

/**
 * @brief Malbolge 仮想機械の状態
 */
class malbolge_machine_state final : public std::enable_shared_from_this<malbolge_machine_state> {
public:
    /**
     * @brief プログラムの実行状態
     */
    enum class ExecutionStatus {
        Running, ///< 実行中
        Exited,  ///< 正常終了
        Aborted  ///< 異常終了
    };

    /**
     * @brief アクセスしようとしたメモリが未初期化であった場合に投げる例外
     */
    class memory_uninitialized_exception {
    public:
        //! 初期化するべきメモリのアドレス
        const malbolge::word address_to_be_set;

        /**
         * @param address_to_be_set 初期化すべきアドレス
         */
        inline memory_uninitialized_exception(const malbolge::word address_to_be_set)
            : address_to_be_set(address_to_be_set)
        {
        }
    };
private:
    //! 親状態へのポインタ
    const std::shared_ptr<malbolge_machine_state> parent = nullptr;

    //! この状態に遷移したときに書き込まれたアドレスとワードの pair。根ノードでは使わない。
    const std::pair<malbolge::word, malbolge::word> written_word;

    //! A レジスタ
    malbolge::word A = 0;

    //! C レジスタ
    malbolge::word C = 0;

    //! D レジスタ
    malbolge::word D = 0;

    //! これまでに出力された文字列
    std::string output;

    //! 親ノードに対するメモリ差分。根ノードでは使わない。
    std::map<malbolge::word, malbolge::word> memory_diffs;

    //! operate() と increment() のうち次に呼ばれるべき方へのポインタ
    ExecutionStatus(malbolge_machine_state::*next_process)() = &malbolge_machine_state::operate;

    /**
     * @brief メモリへのアクセスを試みる
     * @param address アクセスするアドレス
     * @return address 番地のメモリが初期化されている場合はその値。
     * @return 初期化されていない場合は std::nullopt
     */
    std::optional<malbolge::word> check_memory(const malbolge::word address) const;

    /**
     * @brief メモリにアクセスする
     * @param address アクセスするアドレス
     * @return address 番地のメモリが初期化されている場合はその値。
     * @return 初期化されていない場合は例外を投げる。
     * @throws memory_uninitialized_exception address 番地のメモリが初期化されていない
     */
    malbolge::word access_memory(const malbolge::word address) const;

    /**
     * @brief メモリから命令をフェッチし、実行する
     * @return 命令のフェッチと実行を試みた結果の実行状態
     * @throws memory_uninitialized_exception 未初期化のメモリにアクセスした
     * @warning 標準入力から一文字受け取る In 命令の際には問答無用で異常終了する
     */
    ExecutionStatus operate();

    /**
     * @brief メモリの暗号化とレジスタのインクリメントを行う
     * @return メモリの暗号化とレジスタのインクリメントを試みた結果の実行状態
     * @throws memory_uninitialized_exception 未初期化のメモリにアクセスした
     */
    ExecutionStatus increment();

public:
    //! 初期状態からの遷移回数
    const unsigned depth = 0;

    malbolge_machine_state() = default;

    /**
     * @brief 親状態に対し、新たに一か所分メモリに追記された子状態を作る
     * @param ptr_to_parent 親状態へのポインタ
     * @param address 命令を書き込むアドレス
     * @param instruction address 番地に書き込む命令
     */
    inline malbolge_machine_state(
        const std::shared_ptr<malbolge_machine_state> ptr_to_parent,
        const malbolge::word address,
        const malbolge::Instruction instruction
    )
        : parent(ptr_to_parent),
          written_word(address, malbolge::encode_instruction(address, instruction)),
          A(parent->A), C(parent->C), D(parent->D),
          output(parent->output),
          memory_diffs({written_word}),
          next_process(parent->next_process),
          depth(parent->depth + 1)
    {
    }

    /**
     * @return これまでに出力された文字列
     */
    inline std::string_view get_output() const noexcept
    {
        return output;
    }

    /**
     * @brief operate() と increment() のうち次に呼ばれるべき方を呼ぶ
     */
    inline ExecutionStatus process()
    {
        return (this->*next_process)();
    }

    /**
     * @brief 現在の状態へと遷移できる Malbolge コードを生成する
     * @return 現在の状態へと遷移できる Malbolge コード
     * @note 使用されない命令は全て o（Nop 命令）で埋める
     */
    std::string generate_code() const;
};
#endif
