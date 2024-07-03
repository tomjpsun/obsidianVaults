這是 Font Awsome 專案的 Qt 版本，Font Awsome 在許多 node 專案裏面都有用到。
本專案的 GitHub: (https://github.com/gamecreature/QtAwesome)

## 如何使用？

1. 將 GitHub 專案下載後，裏面的子目錄 _QtAwsome_ **copy** 到我們的專案下。

2. qmake 新增這兩行：
```qmake
CONFIG+=fontAwesomeFree
include(QtAwesome/QtAwesome.pri)
```

3. 使用範例：
```c++
#include <QGridLayout>
#include <QPushButton>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtAwesome.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    fa::QtAwesome* awesome = new fa::QtAwesome(qApp);
    awesome->initFontAwesome();
    ui->setupUi(this);

    // button size is 50x50 pixels
    const QSize iconSize(50, 50);
    // set grid layout
    QGridLayout *layout = new QGridLayout();

    // create a button
    QPushButton* btn_cheers = new QPushButton(
        awesome->icon(fa::fa_solid, fa::fa_crown), "Cheers!");

    btn_cheers->setIconSize(iconSize);

    layout->addWidget(btn_cheers, 0, 0);

    // set layout
    ui->centralwidget->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}


```

執行結果如下：

![[Pasted image 20240627102542.png]]

## finish