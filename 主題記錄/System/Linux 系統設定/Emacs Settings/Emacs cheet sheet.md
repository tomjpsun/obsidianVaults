_select word_
快捷鍵 `C-M-SPC`  `C-M-b` 可以選擇游標所在的整個 word

_find matching parenthesis_
`C-M-f` 往下面找 right parenthesis
`C-M-b` 往上面找 left parenthesis

_find caller of a function (under cursor)_
`M-?` : 內建命令
`M-x lsp-find-references`: LSP 命令

eshell 使用 tramp 連線遠端 shell
`M-x eshell`
`cd /ssh:user@remotehost:`

> [!WARNING] 最後面的冒號不能少哦
> 還有, 也可以直接指定到遠端目錄, 例如 `cd /ssh:tom@pi4:/home/tom` , 會直接用 `tramp`連線到遠端 `/home/tom` 目錄下
> 


