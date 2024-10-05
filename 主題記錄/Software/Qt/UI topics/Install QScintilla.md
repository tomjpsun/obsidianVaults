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

這時候會安裝到 `/usr/local/Qt-6.7.2`下面：

```bash
sudo make install
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall -exe libqscintilla2_qt6.so.15.2.1 /usr/local/Qt-6.7.2/lib/libqscintilla2_qt6.so.15.2.1
ln -f -s libqscintilla2_qt6.so.15.2.1 /usr/local/Qt-6.7.2/lib/libqscintilla2_qt6.so
ln -f -s libqscintilla2_qt6.so.15.2.1 /usr/local/Qt-6.7.2/lib/libqscintilla2_qt6.so.15
ln -f -s libqscintilla2_qt6.so.15.2.1 /usr/local/Qt-6.7.2/lib/libqscintilla2_qt6.so.15.2
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/Qsci /usr/local/Qt-6.7.2/include/Qsci
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/qscintilla_cs.qm /usr/local/Qt-6.7.2/translations/qscintilla_cs.qm
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/qscintilla_de.qm /usr/local/Qt-6.7.2/translations/qscintilla_de.qm
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/qscintilla_es.qm /usr/local/Qt-6.7.2/translations/qscintilla_es.qm
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/qscintilla_fr.qm /usr/local/Qt-6.7.2/translations/qscintilla_fr.qm
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/src/qscintilla_pt_br.qm /usr/local/Qt-6.7.2/translations/qscintilla_pt_br.qm
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintilla/qsci /usr/local/Qt-6.7.2/qsci
/usr/local/Qt-6.7.2/bin/qmake6 -install qinstall /home/tom/work/QT_exercises/QScintill
```

# 測試範例

在 Source root 下的 `example` 目錄，用 QtCreator 開啓 `application.pro` ， 執行後看到範例畫面：

![[Pasted image 20240826164542.png]]