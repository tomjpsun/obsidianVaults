---
tags:
  - memory_management
---

[Source from here](https://stackoverflow.com/questions/2491707/memory-management-in-qt/2491976#2491976)


常常看到 Qt new 物件但是沒有 delete？到底 Qt 怎麼管理物件的 delete 呢？
Qt 提供 QObject 內建的機制：Owner 會自動 delete 所有 owned objects， 而且這個 owner 關係與 class 的繼承是**無關**的，例如：

```cpp
QObject* parent = new QObject();
QObject* child = new QObject(parent);
delete parent;  // all the child objects will get deleted when parent is deleted
```

上例 parent , child 同爲 QObject, 但是 child 在 construct 的時候設定了 owner 是 parent object


```cpp
QVBoxLayout* layout = new QVBoxLayout;
QPushButton someButton = new QPushButton; // No owner specified.
layout->addWidget(someButton); // someButton still has no owner.
QWidget* widget = new QWidget;
widget->setLayout(layout); // someButton is "re-parented".
                           // widget now owns someButton.
```

上例，透過  _addLayout()_ 與 _setLayout()_ 的作用，widget 可以變成 someButton 的 owner .

```cpp
QMainWindow* window = new QMainWindow;
QWidget* widget = new QWidget; //widget has no owner
window->setCentralWidget(widget); //widget is now owned by window.
```

上例，_setCentralWidget()_ 也可以設定 owner 關係。
其他會影響 owner 關係的 APIs，參考 Qt 文件。


> [!注意]
> 如果沒有從 QObject 繼承，就回到純 c++ 的做法，要自行 delete[]


