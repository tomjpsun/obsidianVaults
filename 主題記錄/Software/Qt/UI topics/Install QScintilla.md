---
scintella:
---
QScintilla 是一個 Qt Open Source Project ， 提供基本的 editor 功能，基於 Scintilla ，再包一層 Qt，意味着使用者可以 _客製化_ 自己的 editor，有下列特點：
- syntax styling with support for over 70 languages
- error indicators
- code completion
- call tips
- code folding
- margins can contain markers like those used in debuggers to indicate breakpoints and the current line.
- recordable macros
- multiple views
- printing.

安裝如下：
1. 到 QtCreator 取得 qmake 的執行路徑（Preferences > 左邊 Kits > 右邊標籤選  Qt Versions，然後中間有 Kits 的詳情按鈕）
   ![[Pasted image 20240609204311.png]]
2. GitHub Clone [https://github.com/brCreate/QScintilla](https://github.com/brCreate/QScintilla)
4. cd QScintilla/src
5. 執行 qmake 例如： /home/tom/work/Qt/6.7.0/gcc_64/bin/qmake
6. 編譯： make
7. 安裝：make install
