
如果你無法直接取得B物件的實例，仍有幾種方法可以在A物件和B物件之間傳遞資料，這里介紹三種常用的方法：

### 1. 通過全局或靜態物件訪問

如果B物件可以通過某種全局或靜態方式被訪問，你可以利用這個特點來發送數據。這通常不是最佳做法，因為它可能導致代碼難以管理和理解，但在某些情況下，這可能是唯一可行的方法。

### 2. 使用Qt的事件系統

Qt的事件系統（QEvent）允許你發送自定義的事件到任何QObject的子類。這意味著你可以創建一個自定義事件，並且將其發送給B物件（假設你有某種方式來找到B物件或其父物件的指針）。這個方法的好處是它不需要信號和槽的直接連接。

**自定義事件示例：**

首先，定義一個自定義事件類，繼承自`QEvent`。

cppCopy code

解釋

``` C++
class DataEvent : public QEvent {
public:
    static const QEvent::Type EventType = static_cast<QEvent::Type>(QEvent::User + 1);
    explicit DataEvent(const QString &data)
        : QEvent(EventType), mData(data) {}

    QString data() const { return mData; }

private:
    QString mData;
};
```

然後，你可以通過呼叫`QCoreApplication::postEvent()`來發送這個事件給B物件。

cppCopy code

`QCoreApplication::postEvent(bObject, new DataEvent("Hello, World!"));`

在B物件中，你需要重寫`event()`方法來處理這個自定義事件。

cppCopy code

解釋

```C++
bool B::event(QEvent *event) {
    if (event->type() == DataEvent::EventType) {
        DataEvent *dataEvent = static_cast<DataEvent *>(event);
        qDebug() << "Received data in B:" << dataEvent->data();
        return true;
    }
    return QObject::event(event);
}
```

### 3. 使用Qt全局消息系統（如QCoreApplication::sendEvent）

這種方法類似於使用事件系統，但更加全局性。你可以使用`QCoreApplication`來發送事件給任何QObject，即使你沒有直接訪問該物件。這要求你以某種方式能夠獲取或者識別接收事件物件的指針。

在某些情況下，如果B物件能夠以某種方式被系統識別（例如，通過物件名稱），你甚至可以在不直接持有B物件參考的情況下，通過系統查找功能發送事件給它。

這些方法各有利弊，選擇哪一種取決於你的具體需求和應用的架構。在許多情況下，重新考慮設計以允許直接通信（比如通過信號和槽）可能會是更清晰和更可維護的解決方案。