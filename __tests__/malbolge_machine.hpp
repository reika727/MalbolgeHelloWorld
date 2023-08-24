#include "../malbolge.hpp"
#include <istream>
#include <ostream>

/**
 * @brief Malbolge 仮想機械の実装
 */
class malbolge_machine final {
private:
    //! メモリ
    malbolge::word memory[malbolge::word_size];

    //! A レジスタ
    malbolge::word a = 0;

    //! C レジスタ
    malbolge::word c = 0;

    //! D レジスタ
    malbolge::word d = 0;

public:
    /**
     * @param is ソースコードを取得する入力ストリーム
     * @throw std::runtime_error ソースコードが不正
     */
    malbolge_machine(std::istream &is);

    /**
     * @brief 命令を一度実行する
     * @param is 入力ストリーム
     * @param os 出力ストリーム
     * @return プログラムが終了したか否か
     * @throw std::runtime_error メモリの暗号化に失敗した
     * @note 命令のデコードに失敗した場合は何もせずに false を返却する。
     */
    bool exec_one_step(std::istream &is, std::ostream &os);
};
