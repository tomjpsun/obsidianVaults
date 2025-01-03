在 RaspPi 4 用 Emacs lsp 模組, 瀏覽程式碼, 發生上面的錯誤, Emacs 裏面 `C-x` `C-b` 看到 `*clangd::stderr*` 的訊息:

```
...
E[14:49:36.545] Failed to prepare a compiler instance: unknown target ABI 'lp64'
...
```

解決辦法, 在 source 目錄下,新增一個檔案 : `.clangd` 內容是:

```bash
CompileFlags:
  Remove: -mabi=lp64
```

