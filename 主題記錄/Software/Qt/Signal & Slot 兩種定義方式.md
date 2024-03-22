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
