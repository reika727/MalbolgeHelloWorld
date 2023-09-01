/**
 * @file main.cpp
 * @brief main 関数
 */

#include "beam_searcher.hpp"
#include "malbolge.hpp"
#include "malbolge_machine_state.hpp"
#include <iterator>
#include <string>
#include <memory>
#include <algorithm>
#include <random>
#include <iostream>
#include <cctype>

using beam_searcher_t = beam_searcher<std::shared_ptr<malbolge_machine_state>>;

namespace {
    /*
     * 現在の状態から HELLO WORLD という文字列を出力できるか確かめる。
     * ただし探索時間を縮めるため、大文字・小文字の違いは無視する。
     */
    const beam_searcher_t::generation_function_t check_or_generate = [](auto parent, auto bi) {
        // 目標文字列（大文字・小文字の違いは無視する）
        const std::string target = "Hello World";
        try {
            while (true) {
                const auto result = parent->process();
                const auto output = parent->get_output();
                if (result == malbolge_machine_state::ExecutionStatus::Aborted) {
                    // 異常終了したノードは捨てる
                    break;
                } else if (result == malbolge_machine_state::ExecutionStatus::Exited) {
                    // 出力が（大文字・小文字の違いを除いて）一致しているか否か
                    // ここでは文字数だけ比較すればよい
                    return output.length() == target.length();
                } else if (output.length() > target.length()) {
                    // すでに target を超える文字数が出力されてしまっている
                    break;
                } else if (output.length() > 0 && toupper(output.back()) != toupper(target[output.length() - 1])) {
                    // すでに target と一致しない文字が出力されてしまっている
                    break;
                }
            }
        } catch (const malbolge_machine_state::memory_uninitialized_exception &mue) {
            // 未初期化メモリに 8 種類の命令それぞれを代入し、子ノードとする
            for (const auto instruction : malbolge::instructions) {
                *bi++ = std::make_shared<malbolge_machine_state>(parent, mue.get_address_to_be_set(), instruction);
            }
        }
        return false;
    };

    /*
     * なるべく少ない状態遷移で見つけ出すため、
     * 「出力された文字数 × 10 - 遷移回数」
     * をスコアとする。
     */
    const beam_searcher_t::scoring_function_t scoring_function = [](auto node) {
        return static_cast<int>(node->get_output().length()) * 10 - static_cast<int>(node->get_depth());
    };
};

int main()
{
    constexpr unsigned beam_width = 10000;
    beam_searcher_t bs(beam_width, check_or_generate, scoring_function, std::make_shared<malbolge_machine_state>());
    while (!bs.get_current_generation().empty()) {
        std::cout << "GENERATION #" << bs.get_generation() << std::endl;
        std::cout << "\tGENERATION SIZE: " << bs.get_current_generation().size() << std::endl;
        std::cout << "\tBEST RESULT    : " << bs.get_current_generation().front()->get_output() << std::endl;
        std::cout << "\tBEST SCORE     : " << scoring_function(bs.get_current_generation().front()) << std::endl;
        if (
            std::vector<std::shared_ptr<malbolge_machine_state>> found_solutions;
            bs.search_current_generation(std::back_inserter(found_solutions))
        ) {
            std::vector<std::shared_ptr<malbolge_machine_state>> tmp;
            std::ranges::sample(found_solutions, std::back_inserter(tmp), 1, std::mt19937(std::random_device{}()));
            std::cout << std::endl;
            std::cout << "\tFINAL RESULT   : " << tmp.front()->get_output() << std::endl;
            std::cout << "\tFINAL SCORE    : " << scoring_function(tmp.front()) << std::endl;
            std::cout << "\tCODE           : " << tmp.front()->get_code() << std::endl;
            return EXIT_SUCCESS;
        }
    }
    std::cout << "NOT FOUND..." << std::endl;
    return EXIT_FAILURE;
}
