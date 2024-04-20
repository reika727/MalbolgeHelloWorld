/**
 * @file beam_searcher.hpp
 * @brief ビーム探索アルゴリズムの実装
 */

#ifndef BEAM_SEACHER_HPP
#define BEAM_SEACHER_HPP
#include <vector>
#include <span>
#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>

/**
 * @brief ビーム探索アルゴリズムの実装
 * @tparam Node ノードの型
 * @tparam Generator std::uniform_random_bit_generator のモデル
 */
template <class Node, std::uniform_random_bit_generator Generator = std::mt19937>
class beam_searcher final {
public:
    /**
     * @brief 子孫ノードを生成する関数の型
     * @detail 渡されたノードが条件を満たしているならば true を返却する。
     * @detail そうでないならば子ノードを生成し、false を返却する。
     * @param parent 条件を満たしているか検査する親ノード
     * @param bi 子ノードを追加する std::back_insert_iterator
     * @return parent が条件を満たしているか
     */
    using generation_function_t = std::function<bool(Node parent, std::back_insert_iterator<std::vector<Node>> bi)>;

    /**
     * @brief スコア関数の型
     * @param node スコアを計測するノード
     * @return node のスコア
     */
    using scoring_function_t = std::function<int(Node node)>;

private:
    //! ビーム幅
    const std::size_t beam_width;

    //! 子孫ノード生成関数
    const generation_function_t check_or_generate;

    //! スコア関数
    const scoring_function_t scoring_function;

    //! 世代カウント
    std::size_t generation = 1;

    //! 現在の世代
    std::vector<Node> current_generation;

    //! 乱数生成器
    Generator engine;

public:
    /**
     * @param beam_width ビーム幅
     * @param check_or_generate 子孫ノード生成関数
     * @param scoring_function スコア関数
     * @param starting_point 根ノード
     * @param seed 乱数のシード
     * @throws std::runtime_error ビーム幅がゼロ
     */
    beam_searcher(
        const std::size_t beam_width,
        const generation_function_t check_or_generate,
        const scoring_function_t scoring_function,
        const Node starting_point,
        const Generator::result_type seed = std::random_device{}()
    )
        : beam_width(beam_width),
          check_or_generate(check_or_generate),
          scoring_function(scoring_function),
          current_generation({starting_point}),
          engine(seed)
    {
        if (beam_width == 0) {
            throw std::runtime_error("beam_width must not be 0.");
        }
    }

    /**
     * @return 世代カウント
     */
    std::size_t get_generation() const noexcept
    {
        return generation;
    }

    /**
     * @return 現在の世代の読み取り専用 span
     */
    std::span<const Node> get_current_generation() const noexcept
    {
        return current_generation;
    }

    /**
     * @brief 現在の世代に条件を満たすものが存在するか検査しつつ、次の世代を生成する
     * @tparam OutputIterator 出力イテレータの型
     * @param oi 条件を満たしたノードの出力先
     * @return この関数を呼び出した時点での世代に条件を満たすものが存在したか否か。
     * @return 言い換えれば、oi に一つでもノードが出力されたか否か。
     * @note 子ノードが偏るのを防ぐため、ビーム幅に入れるノードのうち同率最下位のものは乱択する。
     */
    template<class OutputIterator>
    bool search_current_generation(OutputIterator oi)
    {
        bool is_found = false;
        std::vector<Node> next_generation;
        for (const auto &parent : current_generation) {
            if (check_or_generate(parent, std::back_inserter(next_generation))) {
                *oi++ = parent;
                is_found = true;
            }
        }
        std::ranges::sort(next_generation, std::ranges::greater(), scoring_function);
        if (next_generation.size() > beam_width) {
            std::ranges::shuffle(
                std::ranges::equal_range(
                    next_generation,
                    scoring_function(next_generation[beam_width - 1]),
                    std::ranges::greater(), scoring_function
                ),
                engine
            );
            next_generation.resize(beam_width);
        }
        ++generation;
        current_generation = std::move(next_generation);
        return is_found;
    }
};
#endif
