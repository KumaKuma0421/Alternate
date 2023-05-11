# Alternate
Visual Studio 2022 C++ Sample Solution

これは、Visual Studio 2022 C++のサンプルソリューションです。
Gitも初心者です。Visual Studioは昔使っていました。最新情報のキャッチアップと昔の手法を新しい技術で実装すると、どうなるのかの研究の下地に使いたいと思っています。

## ConsoleDriver
コマンドラインベースでDLLを実行させるスターターの役目を担います。

## MiddleLibrary
呼び出された後、システム全体を統制する作業を担います。

## WindowsLibrary
Windows APIをある程度クラス化してまとめています。Windowsの場合以下の項目である程度まとまりそうです。

システム全体の構成は、Doxygenで作成した[GitHub Pages](https://kumakuma0421.github.io/Alternate/)を参照してください。

できる限りWindowsのAPI(つまりカーネルDLL)だけ使うようにします。またカーネルコールは昔ながらのハンガリアン記法を使用しますが、ライブラリ提供時点では標準的な変数にしようと思っています。
STLやBOOSTはMiddleLibraryより上位で使うようにして、ここではクラスにWindows APIが便利に使える点だけを求めてみます。

おかしな点やバグと思しきところなどがまだあると思います。見つけた際は、御一報頂けますと幸いであります。