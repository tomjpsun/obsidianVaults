# Install By Debian Package

目前沒有整合好，需要一些手動部分。
首先

	apt install qtcreator qt6-base-dev -y

如果喜歡 clang 就自行安裝 , Debian 12 目前對應到 clang-14 ,  lldb-14

	apt install clangd lldb -y


執行 qtcreator 後：
![[螢幕快照 2023-12-22 15-58-23.png]]

Create Project 後，選右下角 Choose...
![[螢幕快照 2023-12-22 16-00-06.png]]

自行設定 Project Path & 名稱：
![[螢幕快照 2023-12-22 16-12-25.png]]

選擇 qmake
![[螢幕快照 2023-12-22 16-13-54.png]]

最後看到 Kit Selection 全部灰色，沒有可以用的 Kit.


### 如何解決？
按下選單 *編輯* 然後 *Preferences...*, 手動自己選 *Qt Versions*，像這樣：
![[螢幕快照 2023-12-22 16-21-34.png]]

同樣對 *Compilers* 與 *Debuggers* 自己 configure，最後在 Kits summary 看到：

![[螢幕快照 2023-12-22 16-36-17 1.png]]

Qt version 明確選好後，應該可以解決 *沒有 Kits 可以選擇* 的問題了。

附上自己手動的另外一個 Kits 設定，用 clang/llvm 系列:

首先要自己新增 LLDB 的設定名稱：
![[螢幕快照 2023-12-22 16-32-08.png]]

這是 clang 的 Kit 畫面：
![[螢幕快照 2023-12-22 16-53-18 1.png]]

現在，開 Project 就會正常，有 Kits 可以用了：
![[螢幕快照 2023-12-22 16-15-48.png]]

雖然是社羣版本，但是缺乏與 Example 整合。我需要 Example 當作 start point.....

# Install By Qt Official Site : Installer
首先取得 Qt Installer: [[Qt Downloads]]

|  |  |  |
| ---- | ---- | ---- |
|  |  |  |
|  |  |  |
