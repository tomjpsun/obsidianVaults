從 **Edit | Preferences** 下來，default 不會看到 copilit 選項 , 因爲 要從 **Help | 關於外掛程式** 來打開（Qt Creator 是整人大師吧 ？！）

安裝 CompilerExplorer  後，再安裝 copilot plugin 就可以出現選單了，（下圖還沒點選 Copilot ，右邊是 Kits 的畫面）
![[螢幕快照 2024-01-08 15-55-42.png]]



點選 Copilot 設定 Sign in 資訊，node 直接找到 *nodejs* 安裝的路徑，agent 需要從 https://github.com/github/copilot.vim.git 下載 **copilot.vim** ，然後指定它下面的 *dist* 路徑

![[螢幕快照 2024-01-09 10-29-06.png]]

按下 signin ，轉圈圈一會兒，系統 browser 會跳出這個頁面：

![[螢幕快照 2024-01-09 10-24-05.png]]

Activation Code 已經輸入到系統剪貼簿，焦點放在第一個格子，然後 Ctrl-V （paste）即可
ps. 轉圈圈有可能沒有結果，不知道原因。我這邊試到第五次就出現 Activation 畫面，真的很刺激.....

![[螢幕快照 2024-01-09 10-23-13.png]]

當然按 ‘Authorize GitHub Copilot Plugin' ， 然後就看到已經登入（按鈕變成 Sign Out account），記得勾選 Auto Request 下次自動登入！
![[螢幕快照 2024-01-09 10-23-38 1.png]]