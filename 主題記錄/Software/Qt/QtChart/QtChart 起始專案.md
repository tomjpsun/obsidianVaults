首先產生一個專案，然後依照下列修改
#### 手動加入 lib
.pro 檔案：

```
QT       += core gui
QT       += charts
```

#### 加入 headers、宣告變數
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

#### 編輯 ui

1. virtical layout 拖拉到視窗內
2. MainWindow 右鍵選用 `佈局` --> `垂直佈局`

![[Pasted image 20240216170421.png]]

#### layout 裏面開始畫 QChart
```c++

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chart = new QChart();

	// 指定x、y軸刻度，加入 chart 物件
    QValueAxis* axisX = new QValueAxis();
    QValueAxis* axisY = new QValueAxis();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    axisX->setRange(0, 30);
    axisY->setRange(0, 5);

	// 將 chart 物件加入 chartView
    chartView = new QChartView(this);
    chartView->setChart(chart);
    // 將 chartView 加入 ui layout
    ui->verticalLayout->addWidget(chartView);
}
```

結果如下：

![[螢幕快照 2024-02-16 17-01-41.png]]