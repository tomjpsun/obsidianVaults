在專案開發時，遇到用 SIGNAL / SLOT 定義出來的**不會動**，但是用 functor 定義的**卻正常**，查一下文件，發現[這篇說明](https://doc.qt.io/qt-5/signalsandslots-syntaxes.html)

```C++
auto slider = new QSlider(this);
    auto doubleSpinBox = new QDoubleSpinBox(this);

    // OK: The compiler can convert an int into a double
    connect(slider, &QSlider::valueChanged,
            doubleSpinBox, &QDoubleSpinBox::setValue);

    // ERROR: The string table doesn't contain conversion information
    connect(slider, SIGNAL(valueChanged(int)),
            doubleSpinBox, SLOT(setValue(double)));
```
OK 的部分是採用 functor, 於 compile time 就做了，可以隱式轉換型別，因此可以將 int 轉換成 需要 double 的 input
使用 SIGNAL/SLOT 是 String based 定義，runtime 才處理，不能自動轉型別。

比較兩種定義方式：

|                                                                                                   | String-based | Functor-based |
| ------------------------------------------------------------------------------------------------- | ------------ | ------------- |
| Type checking is done at...                                                                       | Run-time     | Compile-time  |
| Can perform implicit type conversions                                                             |              | Y             |
| Can connect signals to lambda expressions                                                         |              | Y             |
| Can connect signals to slots which have more arguments than the signal (using default parameters) | Y            |               |
| Can connect C++ functions to QML functions                                                        | Y            |               |

String based 適合用在跟 QML UI 互動，因爲 QML 彈性大，無法採用 functor 的方式連接 signal / slot