https://github.com/vercel/next.js/tree/canary/examples/with-electron-typescript

#nextjs #electron

依照這個頁面的命令 create：
```bash
npx create-next-app --example with-electron-typescript with-electron-typescript-app
```

得到一個 app 爲 `with-electron-typescript-app`
然後會自動 commit  一份初始的 repo, 但是沒有 .gitignore
所以會在等下 `npm run build` 的時候產生 .js files

依照這個頁面的表格：

|Part|Source code (Typescript)|Builds (JavaScript)|
|---|---|---|
|Next.js|`/renderer`|`/renderer`|
|Electron|`/electron-src`|`/main`|
|Production||`/dist`|

我們需要一份 `.gitignore` 