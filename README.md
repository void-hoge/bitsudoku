# Bit Board Sudoku (Number Place) Solver

## class board
- 数独のデータを保持し、それに対する操作のインターフェースを提供する。
- テンプレート引数はブロックの1辺の長さをとる。
  - 全マス数は、(ブロックの1辺の長さ)^4となる。
  - sudoku::board<3>で雑誌などでよく見る、1辺9マス、全部で81マスの数独になる。

``` C++
template<size_t SIZE>
void sudoku::board<SIZE>::vector_input(const std::vector<int>& q); // std::vectorによるデータ入力
void sudoku::board<SIZE>::cin_input(); // 標準入力からの入力
void sudoku::board<SIZE>::string_input(const std::string& q); // std::stringによる入力
```

- #### vector_input, cin_inputの仕様
  - 空白は0
  - 数字は1以上の整数
  - vector_inputは長さが適切でないものを渡すとstd::logic_errorを投げる。

- #### string_inputの仕様
  - 空白は '-'
  - 数字は1以上の10進整数
  - スペース区切り
  - problems/9x9や problems/16x16に問題例

- #### showの仕様
  - 以下のように表示される。
  - 空白は' '(space)
  - 数字は0以上の16進整数(桁数削減のため)
  - 16進数で2桁以上になるものには対応していない。

```
+------------+------------+------------+------------+
| 0  1  2  3 | 4  5  6  7 | 8  9  a  b | c  d  e  f |
| 4  5  6  7 | 0  1  2  3 | c  d  e  f | 8  9  a  b |
| 8  9  a  b | c  d  e  f | 0  1  2  3 | 4  5  6  7 |
| c  d  e  f | 8  9  a  b | 4  5  6  7 | 0  1  2  3 |
+------------+------------+------------+------------+
| 1  0  3  2 | 5  4  7  6 | 9  8  b  a | d  c  f  e |
| 5  4  7  6 | 1  0  3  2 | d  c  f  e | 9  8  b  a |
| 9  8  b  a | d  c  f  e | 1  0  3  2 | 5  4  7  6 |
| d  c  f  e | 9  8  b  a | 5  4  7  6 | 1  0  3  2 |
+------------+------------+------------+------------+
| 2  3  0  1 | 6  7  4  5 | a  b  8  9 | e  f  c  d |
| 6  7  4  5 | 2  3  0  1 | e  f  c  d | a  b  8  9 |
| a  b  8  9 | e  f  c  d | 2  3  0  1 | 6  7  4  5 |
| e  f  c  d | a  b  8  9 | 6  7  4  5 | 2  3  0  1 |
+------------+------------+------------+------------+
| 3  2  1  0 | 7  6  5  4 | b  a  9  8 | f  e  d  c |
| 7  6  5  4 | 3  2  1  0 | f  e  d  c | b  a  9  8 |
| b  a  9  8 | f  e  d  c | 3  2  1  0 | 7  6  5  4 |
| f  e  d  c | b  a  9  8 | 7  6  5  4 | 3  2  1  0 |
+------------+------------+------------+------------+
```

## class solver
- 深さ優先探索を行う。
- `void solve(board<SIZE> bd, const bool full_search)`で解く。
  - `full_search == true`の時は全解を探索し、`full_search == false`の時は解を一つ見つけた時点で打ち切る。
- `bool is_multiple_solutions(board<SIZE> bd)`は、解が2個発見された時点で探索を打ち切る。
  - 解が一つの時はfalse、解が2つの時はtrueを返す。
- 結果はprivateメンバの`std::vector<board<SIZE>> solved`にpush_backされ、`result()`でそれへの参照が得られる。
- 深さ優先探索で訪れたノード数を`size_t node_count`に記録している。
  - `size_t get_node_count() const`で得られる。

## main.cpp
- ファイルによる入力をとりあえず実装
- コマンドライン引数として、ブロックのサイズ(boardやsolverのテンプレート引数と同じ)、ファイル名を要求する。
- 入力形式は `./bitsudoku [3/4] filename`
- 例えば、`./bitsudoku 3 problems/9x9/allblank.txt`

## 実装した解法
- Naked/Hidden Single
- Hull House (updateに実装)
- X wing (update_xwing, update_xwing_doubleに実装)
- Locked Candidate/Localization (column, row, block) (update_locked_candidateに実装)
- Naked Pair (update_naked_pairに実装、unused)
