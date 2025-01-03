設定新的 emacs init 時，遇到 `copilot`  套件對於 ==restart server 的動作一直報錯==，
因爲以前採用的 https://github.com/copilot-emacs/copilot.el
這個專案已經更新，所以爆出這個問題。

**現在安裝採用`use-package`，需要一併安裝`straight` ，並且在 init.el 開始的時候，需要先把**
**`package-enable-at-startup` disable.**

# Install straight
依照 https://github.com/radian-software/straight.el
的建議，在 **init.el** 開頭就安裝：

``` lisp ln:false
(defvar bootstrap-version)
(let ((bootstrap-file
       (expand-file-name
        "straight/repos/straight.el/bootstrap.el"
        (or (bound-and-true-p straight-base-dir)
            user-emacs-directory)))
      (bootstrap-version 7))
  (unless (file-exists-p bootstrap-file)
    (with-current-buffer
        (url-retrieve-synchronously
         "https://raw.githubusercontent.com/radian-software/straight.el/develop/install.el"
         'silent 'inhibit-cookies)
      (goto-char (point-max))
      (eval-print-last-sexp)))
  (load bootstrap-file nil 'nomessage))
```

# Disable package on startup
簡單，**init.el** 加上
`(setq package-enable-at-startup nil)`
當然是放在 `(require 'package) `之前

# Install use-package
下指令安裝
_M-x package-install [RET]_
_use-package [RET]_
# Install copilot
**init.el** 加上這個即可：
``` ln:false
(use-package copilot
  :straight (:host github :repo "copilot-emacs/copilot.el" :files ("*.el"))
  :ensure t)
```
# Copilot  path
emacs _package install_ ''exec-path-from-shell''

```js
;; init.el 在 package initialize 後加上
(exec-path-from-shell-initialize)
```

