從 **Edit | Preferences** 下來，default 不會看到 copilit 選項 , 因爲 要從 **Help | 關於外掛程式** 來打開（Qt Creator 是整人大師吧 ？！）

勾選 Compiler Explorer 以及 Copilot Plugin 就可以出現 copilot 設定了，（下圖是選擇 **系統選單**|**編輯**|**Preferences...** 後的畫面）

![[Pasted image 20240605090653.png]]

點選 Copilot 設定 Sign in 資訊，**Node.js path** 需要找到 *Node.js* 安裝的路徑，**Path to agent** 需要從 https://github.com/github/copilot.vim.git 下載 **copilot.vim** 專案，然後指定它下面的 *dist* 路徑：

```
Enable Copilot 當然要打勾
Node.js path: /home/tom/.nvm/versions/node/v20.10.0/bin/node
Path to agent.js: /home/tom/work/copilot.vim/dist/language-server.js
Auto request 打勾，下次自動登入
```
完成後如下：

![[Pasted image 20240715142514.png]]

按下 signin ，轉圈圈一會兒，系統 browser 會跳出這個頁面：

![[主題記錄/Software/Qt/Setup/attachments/螢幕快照 2024-01-09 10-24-05.png]]

Activation Code 已經輸入到系統剪貼簿，焦點放在第一個格子，然後 Ctrl-V （paste）即可
ps. 轉圈圈有可能沒有結果，不知道原因。我這邊試到第五次就出現 Activation 畫面，真的很刺激.....

![[主題記錄/Software/Qt/Setup/attachments/螢幕快照 2024-01-09 10-23-13.png]]

當然按 ‘Authorize GitHub Copilot Plugin' ， 然後就看到已經登入（按鈕變成 Sign Out account）
