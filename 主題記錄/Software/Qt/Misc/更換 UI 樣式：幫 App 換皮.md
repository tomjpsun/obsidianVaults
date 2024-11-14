下載 QSS 檔案資源 [Qss templates](git@github.com:GTRONICK/QSS.git) 
當然還有其他的啦，這裏先用來展示**如何將 qss 整合進來**
我想避開檔案的路徑，因爲需要跨平臺使用，所以直接用 **資源檔** 的方式取得 qss.

首先 Qt Creator 的 *File* -> *New File*  爲我們的 project 新增一個 resource file：
![[螢幕快照 2024-01-16 10-59-24.png]]

資源檔案名稱可以自由取，我定爲 **resource.qrc**
然後用文字編輯打進去：

``` css
<RCC>
    <qresource prefix="/">
        <file>stylesheet.qss</file>
        <!-- 其他文件 -->
    </qresource>
</RCC>
```

以後置換新的 .qss，只要名稱改爲 "stylesheet.qss" 就能被 app 使用了。

<補充> : Qt Creator 會幫我們修改 project 設定，新增這個 qrc，像這樣：
``` project file
...
RESOURCES += \
    resource.qrc

```
接下來，直接在 app 開始的時候，採用這個 resource file 即可， 修改我們的 main.cpp 如下：
```c++
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/stylesheet.qss");
    if(file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        q.setStyleSheet(styleSheet);
        file.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
```

最後，就是到第一步驟裏面的 QSS 找自己想用的 stylesheet，這裏使用 MacOS ，在 console 下，將 OSS 裏面的 MacOS.qss copy 到我們的 project 下面：

```bash
# change directory to our project, then
cp ../QSS/MacOS.qss stylesheet.qss 
```

這樣，我們 run time 讀到的 resource，就更換爲 MacOS 風格：

![[螢幕快照 2024-01-16 11-19-38.png]]

因爲是動態載入 stylesheet 所以不需要重新 build. #QtStyleSheet



