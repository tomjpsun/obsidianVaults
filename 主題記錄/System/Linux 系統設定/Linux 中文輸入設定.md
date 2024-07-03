
[參考文件 1](https://www.hkepc.com/forum/viewthread.php?fid=24&tid=2681851&page=1)


> [!NOTE] Install
> $> sudo apt install fcitx5* liblua5.4-dev

**Reboot?**

$> fcitx5

$> im-config

選自己設定（是）
然後出現很多選項，選擇 fcitx5，確定，再確定

$> fcitx5-configtool

在右邊（可用輸入法）
選擇 簡體中文（中國）-> pinyin，把它加入左邊

全域選項最上面，可以看到啟動方式為 Ctrl Space


> [!Warning] fcitx5 自動啓動
> ln -s /usr/share/applications/org.fcitx.Fcitx5.desktop ~/.config/autostart/

**re-login**

此時 Ctrl Space 應該可以進入拼音狀態

**ctrl-shift-f 切換繁體**

這樣就會有繁體拼音輸入法了 

#chinese #fcitx
