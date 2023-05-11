# .editconfigの設定内容について


設定した、.editconfigの内容は以下となる。

## 生成されたドキュメント コメントのスタイル

cpp_generate_documentation_comments = doxygen_double_slash_exclamation

## インデントの設定

### 中かっこのインデント
cpp_indent_braces = false

### 以下を基準に各行をインデントする
cpp_indent_multi_line_relative_to = innermost_parenthesis

### かっこ内で、入力した新しい行を揃える
cpp_indent_within_parentheses = align_to_parenthesis

### 既存のコードで、かっこ内で改行を揃える設定をしない
cpp_indent_preserve_within_parentheses = true

### caseの内容をインデントする
cpp_indent_case_contents = true

### caseラベルをインデントする
cpp_indent_case_labels = true

### caseステートメントの後に中かっこをインデントする
cpp_indent_case_contents_when_block = false

### パラメータとして使うラムダの中かっこをインデントする
cpp_indent_lambda_braces_when_parameter = false

### gotoラベルの位置
cpp_indent_goto_labels = leftmost_column

### プリプロセッサディレクティブの位置
cpp_indent_preprocessor = leftmost_column

### アクセス指定子のインデント
cpp_indent_access_specifiers = false

### 名前空間の内容をインデントする
cpp_indent_namespace_contents = true

### コメントのインデントを維持する
cpp_indent_preserve_comments = true

## 改行の設定

### 名前空間の始めかっこの位置
cpp_new_line_before_open_brace_namespace = new_line

### 型の始めかっこの位置
cpp_new_line_before_open_brace_type = new_line

### 関数の始めかっこの位置
cpp_new_line_before_open_brace_function = new_line

### コントロールブロックの始めかっこの位置
cpp_new_line_before_open_brace_block = new_line

### ラムダの始めかっこの位置
cpp_new_line_before_open_brace_lambda = new_line

### スコープの中かっこを別の行に配置する
cpp_new_line_scope_braces_on_separate_lines = true

### 型が空の場合は、終わりかっこを始めかっこと同じ行に移動する
cpp_new_line_close_brace_same_line_empty_type = false

### 関数の本体が空の場合は、終わりかっこを始めかっこと同じ行に移動する
cpp_new_line_close_brace_same_line_empty_function = false

### 新しい行に'catch'および類似キーワードを配置する
cpp_new_line_before_catch = true

### 新しい行に'else'を配置する
cpp_new_line_before_else = true

### do-whileループの'while'を新しい行に配置する
cpp_new_line_before_while_in_do_while = false

## スペースの設定

### 関数名と引数リストの左かっこの間のスペース
cpp_space_before_function_open_parenthesis = insert

### 引数リストのかっこ内にスペースを挿入する
cpp_space_within_parameter_list_parentheses = false

### 引数リストが空の場合に、かっこ内にスペースを挿入する
cpp_space_between_empty_parameter_list_parentheses = false

### 制御フローステートメント内のキーワードと始めかっこの間にスペースを挿入する
cpp_space_after_keywords_in_control_flow_statements = true

### コントロールステートメントのかっこ内にスペースを挿入する
cpp_space_within_control_flow_statement_parentheses = false

### ラムダ引数リストの始めかっこの前にスペースを挿入する
cpp_space_before_lambda_open_parenthesis = false

### Cスタイルキャストのかっこ内にスペースを挿入する
cpp_space_within_cast_parentheses = false

### Cスタイルキャストの終わりかっこの後にスペースを挿入する
cpp_space_after_cast_close_parenthesis = false

### かっこで囲まれた式のかっこ内にスペースを挿入する
cpp_space_within_expression_parentheses = false

### ブロックの左中かっこの前にスペースを挿入する
cpp_space_before_block_open_brace = true

### 空のかっこの間にスペースを挿入する
cpp_space_between_empty_braces = false

### 均一初期化および初期化子リストの始めかっこの前にスペースを挿入する
cpp_space_before_initializer_list_open_brace = false

### 均一初期化および初期化子リストのかっこ内にスペースを挿入する
cpp_space_within_initializer_list_braces = true

### 均一初期化および初期化子リストの中にスペースを保持する
cpp_space_preserve_in_initializer_list = true

### 始め角かっこの前にスペースを挿入する
cpp_space_before_open_square_bracket = false

### 角かっこ内にスペースを挿入する
cpp_space_within_square_brackets = false

### 空の角かっこの前にスペースを挿入する
cpp_space_before_empty_square_brackets = false

### 空の角かっこの間にスペースを挿入する
cpp_space_between_empty_square_brackets = false

### 多次元配列の角かっこをグループ化する
cpp_space_group_square_brackets = true

### ラムダの角かっこ内にスペースを挿入する
cpp_space_within_lambda_brackets = false

### ラムダの空の角かっこの間にスペースを挿入する
cpp_space_between_empty_lambda_brackets = false

### コンマの前にスペースを挿入する
cpp_space_before_comma = false

### コンマの後にスペースを挿入する
cpp_space_after_comma = true

### メンバー演算子の前後のスペースを削除する
cpp_space_remove_around_member_operators = true

### 型宣言で、基本のコロンの前にスペースを挿入する
cpp_space_before_inheritance_colon = true

### コンストラクターのコロンの前にスペースを追加する
cpp_space_before_constructor_colon = true

### セミコロンの前のスペースを削除する
cpp_space_remove_before_semicolon = true

### セミコロンの後にスペースを挿入する
cpp_space_after_semicolon = true

### 単項演算子とそのオペランドの間のスペースを削除する
cpp_space_remove_around_unary_operator = true

### バイナリ演算子の前後のスペース
cpp_space_around_binary_operator = insert

### 代入演算子の前後のスペース
cpp_space_around_assignment_operator = insert

### ポインター/参照の配置
cpp_space_pointer_reference_alignment = left

### 条件演算子の前後のスペース
cpp_space_around_ternary_operator = insert

## 折り返しオプション

### ブロックの折り返しオプション
cpp_wrap_preserve_blocks = one_liners

[戻る](.\MyRule.md)