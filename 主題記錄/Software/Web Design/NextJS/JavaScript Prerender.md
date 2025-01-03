# 主要參考來源
[官方文件](https://nextjs.tw/)


### Disable JavaScript rendering

Chrome `[F12]` 出現 developer environment
`Ctrl-Shift-P` 出現命令選單，key in ‘JavaScript’，
會出現 '停用 JavaScript' 按 return 採用這個命令，
此時瀏覽器的瀏覽列 圖示會出現紅色X 表示本頁面不執行 JavaScript 了
![[Pasted image 20230905110521.png]]
https://create-react-template.vercel.app/
這是 react app 範例，在這裏進行 disable JavaScript 會讓 app 無法順利顯示完


https://next-learn-starter.vercel.app/
這是 nextJS 範例，在這裏進行 disable JavaScript 不會影響顯示， 那是因為 Next.js 會將應用程式提前渲染成 HTML，讓你可以不需要執行 JavaScript 就可以看到應用程式的介面。

詳細說明在這裏:https://nextjs.tw/learn/basics/data-fetching/pre-rendering

