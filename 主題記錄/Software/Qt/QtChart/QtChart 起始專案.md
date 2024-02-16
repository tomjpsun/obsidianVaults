首先產生一個專案，然後依照下列修改
#### 手動加入 lib
.pro 檔案：

```
QT       += core gui charts
```

#### 加入 headers
```cpp

#include <QMainWindow>
#include <QChartView>
#include <QChart>
class MainWindow : public QMainWindow
{
...    

	QChartView* chartView;
    QChart* chart;

}
```


