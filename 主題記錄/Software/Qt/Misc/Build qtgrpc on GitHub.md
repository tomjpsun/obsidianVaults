## 前言

本篇是針對 qtgrpc 專門 build，屬於實驗性質，因爲實驗了 ninja build 的指令。
在 Qt 整體專業裏面，qtgrpc 是一個子專案。

比較建議的方式，是透過整體 build 的方式，順便將 qtgrpc build 出來，這些過程記錄在另一篇：[[Build Qt-6.7.2 From Source in Linux]]

## First: Build qtbase
需要 packages
cmake, git, clang,libgl1-mesa-dev, libglu1-mesa-dev, ninja-build, libfontconfig1-dev (for fonts to be rendered correctly), libinput-dev (for the XCB platform plugin)

```bash
git clone git@github.com:qt/qtbase.git
cd qtbase
cmake -Bbuild -GNinja
cd build
ninja
sudo ninja install
# 接下來許多檔案安裝到 /usr/local/Qt-6.9.0/
```

## Build qtgrpc

需要知道 qtbase 裝到哪裏了？ 把它 export 出來，後面其他 package 要用：

```bash
export Qt6_DIR=/usr/local/Qt-6.9.0/lib/cmake/Qt6
```

```bash
clone git@github.com:qt/qtgrpc.git
cd qtgrpc
cmake -GNinja -Bbuild
cd build
ninja
sudo ninja install
```

 _打完收工_

