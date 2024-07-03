# 取得  CQtDeployer

專案位置：
https://github.com/QuasarApp/CQtDeployer?tab=readme-ov-file

這是可以做出 Windows 與 Linux 部署的工具。
## 安裝：
### 直接用安裝檔
到[這裏](https://github.com/QuasarApp/CQtDeployer/releases)下載，因爲我是 Debian ，下載 .deb 然後安裝。
好了以後，就可以執行

``` bash
$> cqtdeployer -bin <AppBin>
```

其中  **\<AppBin\>** 是用 **Release build** 的 App，直接從 Project 目錄下面 copy 出來。

### Build from source
因爲牽涉其他的 git submodules:
```bash
$> git clone --recursive git@github.com:QuasarApp/CQtDeployer.git
```

進入 CQtDeployer, create 一個 build 目錄，然後用 cmake build

```bash
$> cd CQtDeployer
$> mkdir build
$> cd build
$> cmake ..
$> make
```
build 完成後，在 build/src/CQtDeployer/ 裏面會有一個同名的執行檔。( 沒有實驗過 sudo make install 會怎樣 )

## 製作：

以一個 Project **Ptp4l** 爲例，就是在 **Ptp4l/build/Desktop_Qt_6_7_0-Release** 下面，將之 copy 到 ~/Download/ 後，在 ~/Download 目錄下執行：

```bash
$> cqtdeployer -bin Ptp4l
```

就做出：
Download
├── DistributionKit
├── Ptp4l
│   ├── bin
│   │   ├── Ptp4l
│   │   └── qt.conf
│   ├── lib
│   │   ├── libicudata.so.73
│   │   ├── libicui18n.so.73
.
.
.
│   ├── Ptp4l.sh
.
.
.
(other libraries...)

這樣的結構， DistributionKit 可以直接發佈給 user。
user 只要執行 **Ptp4l.sh** 就可以即時設定環境，並執行 App了。






