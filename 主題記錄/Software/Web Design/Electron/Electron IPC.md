[原文出處](https://ithelp.ithome.com.tw/articles/10235110)
# [ Day 9 ] - 動物聊天室(二) - IPC 與訊息交換

第三天時 , 本魯提過 IPC 這個名詞 , 並說這是 Main Process 與 BrowserWindow 做溝通訊息的方式

![](https://i.imgur.com/HwueFRu.png)

### 那 IPC 是什麼 ?

> 行程間通訊（IPC，Inter-Process Communication），指至少兩個行程或執行緒間傳送資料或訊號的一些技術或方法。

以上說明來自 WIKI , 也就是說 Main Process 與 BrowserWindow 傳送資料的方法就是 IPC

### 在 Electron 中 IPC 如何實作 ?

Electron 提供了 IpcRenderer 與 IpcMain 方便工程師實作 IPC

- 畫面端 ( BrowserWindow ) 用 IpcRenderer
- 主處理序端 ( Main Process ) 用 IpcMain

### 實際代碼如何撰寫 ?

下方展示 3 種溝通模式

- 別人來接

由 IpcMain.on 監聽 IpcRenderer.send 傳來的訊息
之後用 event.reply 回傳
說人話 : 你將小貓帶回家後 , 你請媽媽幫忙照顧你的小貓 , 由她幫忙餵食

![[Pasted image 20231213154023.png]]

```javascript
// main.js - 在主處理序裡。
const { ipcMain } = require('electron')
ipcMain.on('take-cat-home-message', (event, arg) => {
  console.log(arg) // prints "帶小貓回家"
  event.reply('need-clean-reply', '貓咪肚子餓')
})

// preload.js - 在畫面轉譯處理序中 (網頁)。
const { ipcRenderer } = require('electron')
ipcRenderer.on('need-clean-reply', (event, arg) => {
  console.log(arg) // 印出 "貓咪肚子餓"
})
ipcRenderer.send('take-cat-home-message', '帶小貓回家')

```

- 等待回應
    你一直在盯著小貓怕她肚子餓 , 這中間你不做任何事情

![[2E7ONWb.png]]

```javascript
// main.js - 在主處理序裡。
const { ipcMain } = require('electron')
ipcMain.on('take-cat-home-message', (event, arg) => {
  console.log(arg) // prints "帶小貓回家"
  // event 回傳前你一直關注著小貓
  event.returnValue = '小貓肚子餓'
})

// preload.js - 在畫面轉譯處理序中 (網頁)。
const { ipcRenderer } = require('electron')
console.log(ipcRenderer.sendSync('take-cat-home-message', '帶小貓回家')) // prints "小貓肚子餓"
```

- 通知回應
    你打掃家裡，小貓肚子餓會 "喵喵叫"

![[i8nNnBR.png]]

```javascript
// main.js - 在主處理序裡。
const { ipcMain } = require('electron')
ipcMain.handle('take-cat-home-handle', async (event, arg) => {
  console.log(arg) // prints "帶小貓回家"
  return '小貓肚子餓，喵喵叫'
})

// preload.js - 在畫面轉譯處理序中 (網頁)。
const { ipcRenderer } = require('electron')
ipcRenderer.invoke('take-cat-home-handle', '帶小貓回家')
           // then 回傳前可以做其他事，例如打掃家裡
           .then(msg => console.log(msg)) // prints "小貓肚子餓，喵喵叫"
```

上方 3 種方式都是由 畫面端開始 , 如果要由 主處理序 開始觸發消息要如何做 ?

![[RnmBjcM.png]]

```javascript
// main.js - 在主處理序裡。
mainWindow.webContents.send('switch-cat', number);

// preload.js - 在畫面轉譯處理序中 (網頁)。
const { ipcRenderer } = require('electron')
ipcRenderer.on('switch-cat', (event, args) => switchCat(args));
```

不知道上方程式碼 , 邦友有沒有覺得熟悉 ?