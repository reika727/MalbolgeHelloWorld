/**
 * @file beam_searcher.hpp
 * @brief ビーム探索アルゴリズムの実装
 */

#ifndef BEAM_SEACHER_HPP
#define BEAM_SEACHER_HPP
#include <vector>
#include <span>
#include <optional>
#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>

/**
 * @brief ビーム探索アルゴリズムの実装
 * @tparam Node ノードの型
 */
template <class Node>
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
    const unsigned beam_width;

    //! 子孫ノード生成関数
    const generation_function_t generate_child;

    //! スコア関数
    const scoring_function_t scoring_function;

    //! 世代カウント
    unsigned generation = 1;

    //! 現在の世代
    std::vector<Node> current_generation;

public:
    /**
     * @param beam_width ビーム幅
     * @param generate_child 子孫ノード生成関数
     * @param scoring_function スコア関数
     * @param starting_point 根ノード
     * @throws std::runtime_error ビーム幅がゼロ
     */
    beam_searcher(
        const unsigned beam_width,
        const generation_function_t generate_child,
        const scoring_function_t scoring_function,
        const Node starting_point
    )
        : beam_width(beam_width),
          generate_child(generate_child),
          scoring_function(scoring_function),
          current_generation({starting_point})
    {
        if (beam_width == 0) {
            throw std::runtime_error("beam_width must not be 0.");
        }
    }

    /**
     * @return 世代カウント
     */
    unsigned get_generation() const noexcept
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
     * @return 現在の世代に条件を満たすものが存在するならばそれを返却する。
     * @return そうでない場合は current_generation を次の世代で置き換え、std::nullopt を返却する。
     * @note 子ノードが偏るのを防ぐため、ビーム幅に入れるノードのうち同率最下位のものは乱択する。
     */
    std::optional<Node> search_current_generation()
    {
        std::vector<Node> next_generation;
        for (const auto &parent : current_generation) {
            if (generate_child(parent, std::back_inserter(next_generation))) {
                return parent;
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
                std::mt19937(std::random_device()())
            );
            next_generation.resize(beam_width);
        }
        ++generation;
        current_generation = std::move(next_generation);
        return std::nullopt;
    }
};
#endif