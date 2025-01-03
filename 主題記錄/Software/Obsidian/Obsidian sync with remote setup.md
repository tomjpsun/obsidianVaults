# Syncthing AppImage 安裝
1. use 'Syncthing': download Appimage from https://syncthing.net/downloads/
2. chmod the Appimage, then run it
# Syncthing debian package 安裝
```bash
sudo apt install syncthing syncthing-gtk
```

# Syncthing 設定
1. 執行後可以看到基本設定：

![[Pasted image 20230807163712.png]]
2. 進入資料夾-編輯資料夾-設定資料夾路徑，這裏例子爲”/home/tom/documents/Obsidian Vault" , 這個路徑是 Obsidian 安裝時，我們設定的路徑：

![[Pasted image 20230808110932.png]]
3. 儲存完成資料路徑設定。

# remote_device
在 syncthing 畫面右下角，按下‘新增遠端裝置‘ 按鈕，然後把遠端的"裝置識別碼” 打進去：

![[2023-08-08 13-06-59 的螢幕擷圖 2.png]]


已共享的資料夾

![[2023-08-08 13-07-31 的螢幕擷圖.png]]


# port
Syncthing 是以機器爲 sync 的基礎，首先我們可以使用

netstat -anpe | grep "syncthing" | grep "LISTEN"

指令找出 syncthing service 的 port number, 我看到爲 8384:

tcp        0      0 127.0.0.1:8384          0.0.0.0:*               LISTEN      1000       65772      2980/syncthing
tcp6       0      0 :::22000                :::*                    LISTEN      1000       35091      2980/syncthing


# Obsidian 設定

1. 打開 Obsidian Settings（左下角齒輪）in 'Files & Links'  在 'Default location for new notes' 選擇 'In the folder specific below' ，此時就會多一個設定欄位，可以指定新的 notes 存放在這個路徑下 ;  'Default location for new notes' 的設定也是同樣意思。我在這裏把他們分別設定爲 '/home/tom/documents/ObsidianVault/diary'與 '/home/tom/documents/ObsidianVault/assets' :

![[Pasted image 20230808124544.png]]
![[2023-08-08 11-28-48 的螢幕擷圖.png]]

這樣就可以把檔案利用 syncthing 與遠端同步了。

## Obsidian GitHub ##

另外可以用 github plugin 同步，這個適合給 iDevice 使用
https://blog.poychang.net/obsidian-sync-between-desktop-and-mobile-with-git/

