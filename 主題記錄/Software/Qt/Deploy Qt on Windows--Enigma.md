 原文出處：[https://moon-half.info/p/4192](https://moon-half.info/p/4192)

# QT 學習筆記 – 程式佈署與 enigma virtual box 打包

以QT Creator 開發的程式，一般在編譯後都是以動態連結的方式在執行期才與需要的 .dll 做連結，所以如果沒做特別的處理，並無法單獨執行，一定要在開啟 IDE 的狀態執行才可以。如果你試著在輸出目錄執行 exe 檔的話，應該會看到類似下列的錯誤：找不到Lib

[![](https://i2.wp.com/moon-half.info/wp-content/uploads/2021/04/loadfailed.jpg?resize=602%2C351&ssl=1)](https://i2.wp.com/moon-half.info/wp-content/uploads/2021/04/loadfailed.jpg?ssl=1)
###  QT 佈署工具 windeployqt
這個工具是跟 Qt 一同安裝的，在 Qt 6.7.0 ，是放在 Qt 目錄下的 *Qt/6.7.0/Src/qtbase/src/tools/windeployqt*

#### 編譯輸出位置

目前試出的結果是**只能對 Release 設置做打包**，如果是用 Debug 的話，則會失敗。編譯的設定，可以直接在左下角做修改。

[![](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/qtbuildtype.jpg?resize=283%2C300&ssl=1)](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/qtbuildtype.jpg?ssl=1)

Build Type : Release

接著，就要找出程式編譯的輸出目錄。在「專案」設定的圖示按一下後，就可以看到目前的檔案輸出目錄了。

[![](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/qtprjconf.jpg?resize=300%2C212&ssl=1)]
(https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/qtprjconf.jpg?ssl=1)

[![](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/qtoutputdir.jpg?resize=300%2C79&ssl=1)](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/qtoutputdir.jpg?ssl=1)

輸出目錄

看一下輸出目錄下的 Release 目錄，就可以看到編譯出的檔案。

[![](https://i2.wp.com/moon-half.info/wp-content/uploads/2021/04/outputdirfiles.jpg?resize=300%2C82&ssl=1)](https://i2.wp.com/moon-half.info/wp-content/uploads/2021/04/outputdirfiles.jpg?ssl=1)

編譯出的檔案

這邊就可以看到 .exe 檔了，但是還不能執行，下一步就是將所需的動態連結檔收集起來。

#### QT 指令環境

要執行打包程式，可以利用 QT 的指令環境。在搜尋的地方打上 QT，就會列出 “QT 5.6 for Desktop…”，執行該項目就會跳到DOS視窗。
[![](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/qtcmdenv.jpg?resize=602%2C475&ssl=1)](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/qtcmdenv.jpg?ssl=1)

QT 指令環境

接著將目錄切換到剛的 .exe 位置，再執行 windeployqt . ，就會進行所需檔案收集的的動作。[![](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/collection.jpg?resize=602%2C381&ssl=1)](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/collection.jpg?ssl=1)

檔案收集
#### 再次執行檔案

當收集完檔案後，可以看到編譯輸出的目錄，多了很多檔案。

[![](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/collectionafter.jpg?resize=602%2C464&ssl=1)](https://i1.wp.com/moon-half.info/wp-content/uploads/2021/04/collectionafter.jpg?ssl=1)

收集到的檔案

這時，再執行 .exe 就可以成功執行，不會再提示缺少 .dll 檔案了。

### 用 enigma virtual box 將程式打包成單一執行檔

目前程式以可以獨立執行，不需要再安裝 QT 的開發環境了。但一堆檔案看起來總是不太舒服，這時我們可以利用 [enigma virtual box](https://enigmaprotector.com/en/aboutvb.html) 這個工將所有的程式打包成一個執行檔，而不是以目錄來儲存所有所需的檔案。**這是個免費工具**，可以直接下載使用。請到官網 [https://enigmaprotector.com/en/downloads.html](https://enigmaprotector.com/en/downloads.html)進行下載安裝。Attachement 目錄附上我使用的版本，以防連結失效。

Enigma Virtualbox

安裝完打開程式，先設定幾個部份

[![](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/eopen.jpg?resize=602%2C422&ssl=1)](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/eopen.jpg?ssl=1)

Enigma 程式

1. 選定主要要執行的檔案，也就是 QT Build 出來的執行檔
2. 打包後要輸出的執行檔位置和名稱，這可以依自己喜好設定
3. 按下 「Files Options」，會跳出(4)的對話框，勾選壓縮檔案，這樣會小一點。

接下來按下「Add」選擇 Add Folder Recursive。這時就是要選擇把哪些其它的檔案一起打包，由於剛剛 QT 執行檔所需要的 dll 檔都被與執行檔放在了一起，所以直接選執行檔所在的目錄就可以了。以本文的例子就是 **D:\build-qtdep-Desktop_Qt_5_6_2_MinGW_32bit-Release\release** ，然後 「Select Folder」對話框選擇用預設值按 ok 即可。

展開檔案樹，看起來的畫面就會像下面這樣。

[![](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/eniaddfiles.jpg?resize=300%2C296&ssl=1)](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/eniaddfiles.jpg?ssl=1)

加入額外檔案

最後按下 「Process」，就會進行打包壓縮，完成後就會產生出單一的可執行檔案了！

[![](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/process.jpg?resize=602%2C423&ssl=1)](https://i0.wp.com/moon-half.info/wp-content/uploads/2021/04/process.jpg?ssl=1)

打包壓縮中
### 執行檔案

雖然是打包成單一檔案，不過在執行時應該還是偷偷在背景解壓縮。所以第一次啟動會比較慢，第二次速度就會提高了。一個空程式，打包後的大小約是 17MB，雖然有點大，不過使用上沒什麼問題。如果很在意大小，可以試著不要將某些 dll 加入，例如 opengl32sw.dll 這個檔案好像就是不需要的，去除的話檔案可以減少到12MB。
