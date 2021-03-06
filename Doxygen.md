# Doxygenコメント

## ファイルへのコメント

|キーワード|説明|
|:--------|:---|
|@file    |ファイル名|
|@brief   |簡単な説明|
|@author  |最初に書いた人の名前|
|@date    |ファイルを起こした日付|
|@details |詳細な説明|

## 名前空間へのコメント(通常は省略)

|キーワード|説明|
|:--------|:---|
|@namespace|名前空間の説明|

## クラス、構造体へのコメント

|キーワード|説明|
|:--------|:---|
|@class   |クラス名|
|@struct  |構造体名|
|@brief   |詳解|
|@note    |詳解の下に「覚え書き」として表示される|

## 関数へのコメント
以下のDoxygenコメントはヘッダーファイル側に記載します。実態はソースコード側にもありますが、そちらには記載しません。
（バイナリ配布の際、DLL、LIB、Headerまではライブラリとして同梱しますが、ソースコードは同梱しない場合もあるため）

|キーワード|説明|
|:--------|:---|
|@brief   |この関数の概要を説明|
|@param   |パラメータの説明|
|@param[in]|パラメータ(in)の説明|
|@param[out]|パラメータ(out)の説明|
|@param[in,out]|パラメータ(in, out)の説明|
|@return  |戻り値の説明|
|@retval  |戻り値の値の説明|
|@sa      |元にした情報のURL、参照すべき関数など|
|@details |@briefに入りきれない詳細な説明|
|@attention|注意書き|
|@warning|警告|
|@note|覚書|
|@code～@endcode|コードがそのまま出力される|
|@overload|オーバーロード関数の関数コメントに付けて、ドキュメント出力時にオーバーロードがあることを示す|
|@exception|発生する例外のリスト|

@fnは冗長になるので書きません。

## 変数のコメント

```
//! 変数のコメント
int hensu = 0;
```

```
int hensu = 0; //!< 変数の後ろに記載する場合
```

## マクロのコメント

```
//! @def マクロのコメント
#define MAX_NANIKA 256
```

## 列挙体のコメント

```
//! @enum Enum 列挙体への説明
enum Enum {
    //! 列挙体の各要素へのコメント
    EnumItem1 = 0x00
}
```

# 特殊コマンド

@mainpage Doxygenのトップページの内容を記載
@page ドキュメントページ
@section ページ内のセクション

# 便利なDoxygen構文

## リスト

マイナス記号をカラム位置を揃えて使うことにより、黒丸付きリストが生成される。

マイナス記号の後にシャープを置くことで、数字付きリストが生成される。

## HTMLコマンド

直接HTMLタグを使用することができる。この場合のインデントは重要ではない。