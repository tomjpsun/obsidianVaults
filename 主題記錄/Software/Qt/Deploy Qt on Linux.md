## 取得  CQtDeployer

專案位置：
https://github.com/QuasarApp/CQtDeployer?tab=readme-ov-file

這是可以做出 Windows 與 Linux 部署的工具。
## 安裝：
#### 直接用安裝檔
到[這裏](https://github.com/QuasarApp/CQtDeployer/releases)下載，因爲我是 Debian ，下載 .deb 然後安裝。
好了以後，就可以執行

``` bash
$> cqtdeployer -bin <AppBin>
```

其中  **\<AppBin\>** 是用 **Release build** 的 App，直接從 Project 目錄下面 copy 出來。

#### Build from source
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

## 製作 App：
#### qmake 環境變數
如果安裝不同版本的 Qt，或者不清楚 qmake 目前管理的狀況，到底是採用那一個版本？這裏用環境變數來設定，就可以修正。

	qtchooser -install <qt version> <qmake path>

例如我們用 Qt 6.7.1：

	qtchooser -install qt6.7.1 ~/work/Qt/6.7.1/gcc_64/bin/qmake

透過命令 **qtchooser -print-env** 可以驗證:

	QT_SELECT="qt6.7.1"
	QTTOOLDIR="/home/tom/work/Qt/6.7.1/gcc_64/bin"
	QTLIBDIR="/home/tom/work/Qt/6.7.1/gcc_64/lib"

如果看到上面的樣式（qt version number 各自表述) 就 OK 啦，接着 .bashrc 最後加上
`export QT_SELECT=qt6.7.1` （qt version number 各自表述)
#### cqtdeployer 參數
以一個 Project **Production** 爲例，就是在 **Production/build/Desktop_Qt_6_7_1-Release** 下面，將之 copy 到 ~/Download/ 後，在 ~/Download 目錄下執行：

```bash
cqtdeployer -bin Production -qmake /home/tom/work/Qt/6.7.1/gcc_64/bin/qmake
```

就做出：
Download
├── DistributionKit
├── Production
│   ├── bin
│   │   ├── Production
│   │   └── qt.conf
│   ├── lib
│   │   ├── libicudata.so.73
│   │   ├── libicui18n.so.73
.
.
.
│   ├── **Production.sh**
.
.
.
(other libraries...)

這樣的結構， DistributionKit 可以直接發佈給 user。
user 只要執行 **Production.sh** 就可以即時設定環境，並執行 App了。

#### Deploy The Shell Script

Copy 到 target system 後，對着 .sh 按右鍵，選擇 `允許作爲程式執行`:



然後在 `~/.config/autostart/` 下，新增一個
App.desktop (主檔名自由取，副檔名一定要是 .desktop)
內容如下:

```
[Desktop Entry]
Name=MyScript
GenericName=A descriptive name
Comment=Some description about your script
Exec=/home/etd/Documents/DistributionKit/Production.sh
Terminal=false
Type=Application
```

其中 `Exec` 必須指定到我們的 shell script。這樣當我們 login 的時候，就會**自動啓動**程式。 #
