在 runtime 下出現如下警告：
QObject: Cannot create children for a parent that is in a different thread.
(Parent is QSerialPort(0x7ffdc97c40a0), parent's thread is QThread(0x5628888f3d50), current thread is ProgrammableMemoryManager(0x7ffdc97c4040)

按照字面意思是：不能爲屬於不同的線程的 parent object 創建 (child) object，詳細探討參考：
https://blog.csdn.net/qq_35511927/article/details/121422908



q