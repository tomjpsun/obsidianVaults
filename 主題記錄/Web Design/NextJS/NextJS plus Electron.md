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

我們需要一份 `.gitignore`:

```js
# See https://help.github.com/articles/ignoring-files/ for more about ignoring files.

# dependencies
/node_modules
/.pnp
.pnp.js

# testing
/coverage

# electron
/main/

# next.js
/renderer/out/
/renderer/.next/

# production
/build
/dist

# misc
.DS_Store
*.pem

# debug
npm-debug.log*
yarn-debug.log*
yarn-error.log*
.pnpm-debug.log*
/dev/

# local env files
.env*.local
# vercel
.vercel

# typescript
*.tsbuildinfo
next-env.d.ts
/.github/
.vscode/snipsnap.code-snippets

```