
[參考文件 1](https://www.hkepc.com/forum/viewthread.php?fid=24&tid=2681851&page=1)

## 自己的筆記
$> sudo apt install fcitx5* liblua5.4-dev

reboot

$> fcitx5

$> im-config:

選自己設定（是）
然後出現很多選項，選擇 fcitx5，確定，再確定

## 設定起始程式

打開 console 執行 `gnome-tweaks`
在 `初始啓動應用程式`項目裏面, 按 + 號, 新增 fcitx5:
![[Pasted image 20241122132730.png]]


$> fcitx5-configtool：

在右邊（可用輸入法）
選擇 簡體中文（中國）-> pinyin，把它加入左邊

全域選項最上面，可以看到啟動方式為 Ctrl Space

重新登入

此時 Ctrl Space 應該可以進入拼音狀態
ctrl-shift-f 切換繁體

這樣就會有繁體拼音輸入法了
#chinese #fcitx
