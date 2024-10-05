---
qt-build:
---

來源參考[這篇 Qt 文件](https://doc.qt.io/qt-6/build-sources.html)

首先安裝一些需要的 packages：

```bash
$> sudo apt install cmake libgl1-mesa-dev libglu1-mesa-dev g++ libfontconfig1-dev libfreetype-dev libx11-dev libx11-xcb-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libxcb-cursor-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-util-dev libxcb-xinerama0-dev libxcb-xkb-dev libxkbcommon-dev libxkbcommon-x11-dev libclang-dev python3-html5lib gperf bison flex
```

_Note:_ 上面 libclang-dev 是因爲要修正錯誤，而補安裝的套件，錯誤訊息如下：
```
CMake Error at /usr/lib/llvm-14/lib/cmake/clang/ClangTargets.cmake:756 (message):
  The imported target "clangBasic" references the file

     "/usr/lib/llvm-14/lib/libclangBasic.a"
```

到 [download.io]($dir$下載 _qt-everywhere-src-6.7.2.tar.xz_ 解開後開始build

```bash
mkdir -p ~/dev/qt-build
cd ~/dev/qt-build
# webengine 一直有問題，先 build 其他模組就好
/tmp/qt-everywhere-src-6.7.2/configure -skip qtwebengine

# ....等 configure 結束，不要用 --paralled 因爲虛擬機，會等不到回應而報錯
cmake --build . 
sudo make install
```

這裏記錄一下最後結果：
```
[10105/11020] Automatic MOC for target SerialBus
AutoMoc: /home/tom/work/qt-everywhere-src-6.7.2/qtserialbus/src/serialbus/qcanframeprocessor.h: note: No relevant classes found. No output generated.
[11020/11020] Linking CXX shared modul...l/QtWebView/libqtwebviewquickplugin.so

real    93m36.064s
user    173m13.010s
sys     12m54.550s

```
花了 1.5 hour，在 2 核心的 virtual 環境下。

## 使用
### Add Qt PATH
```bash
#在 .bashrc 加入
PATH=/usr/local/Qt-6.7.2/bin:$PATH
export PATH
source ~/.bashrc
```
### Install QtCreator
```bash
sudo apt install qtcreator
```
### Add Qt Version
這時候安裝的 Qt Creator 並沒有可使用的 Qt version，到 Edit | Preferences => Kits => Add 一個新的，路徑採用上面新增的 Qt PATH

![[Pasted image 20240625092743.png]]


照這裏加入新的  _Qt Version qmake_:
![[Pasted image 20240625091824.png]]

路徑採用前面 build 完成的 /usr/local/Qt-6.7.2/bin/qmake，確定後就可以了。
##### finish
