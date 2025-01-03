1. 到您的 Obsidian 文件資料夾下命令 --> `git init`
2. 打開 Settings --> Community Plugin --> 瀏覽 Obsidian Git 插件， 安裝起來
3. ![[Pasted image 20230915090823.png]]

進入設定
+ `啓用` 後 `選項`
+  或者按 `齒輪`

#### Split automatic commit and push
分兩種自動備份 git 方式: `1.定時備份` `2.編輯一篇文章後備份`
Enable 就是選用上面的定時備份，
我採用第二種，下面說明也是第二種。所以這裏是 Disable. (第一種請自行測試)

#### Vault Backup interval(minutes)
當`停止編輯`後幾分鐘，算是完成編輯。我設定 5 min

#### Auto Backup after stop editing any file
判斷`停止編輯`時自動備份，我這裏 Enable

跳到

#### Pull updates on startup
我把他 Enable，沒試驗過。但是應該是打開 Obsidian 後自動抓 git 回來 sync
因爲我另一臺電腦也會編輯，所以此選項 Enable.

其餘部分都採用 default, 備份後長這樣:

``` bash
tom@omega:~/documents/ObsidianVault$ git lg
* fc5a70a - (HEAD -> master) vault backup: 2023-09-15 09:27:16 (14 分鐘前) <Ching Ping Sun>
* 238d068 - vault backup: 2023-09-15 09:24:19 (17 分鐘前) <Ching Ping Sun>
* 8bea947 - vault backup: 2023-09-15 09:23:16 (18 分鐘前) <Ching Ping Sun>
* 8f80d6e - initial commit (40 分鐘前) <Ching Ping Sun>
```

分享完畢，多一層保險啦！