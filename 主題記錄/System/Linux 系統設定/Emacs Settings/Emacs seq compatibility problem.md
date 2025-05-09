`magit` 是 emacs 必裝套件，但是自從 `seq` v2.24 後就不相容，作者堅持不接受`seq` v2.24 的 default settings

這裏有升級操作，避免後續一直冒出warnings：
- M-x `package-refresh-contents`
- M-x `list-packages`
- /n `seq` to filter by name.
- i to mark a package for installation.
- x to execute your marked changes.

[Ref](https://emacs.stackexchange.com/questions/80871/how-to-provide-updated-seq-package-to-magit)

# RPI 下安裝 emacs 環境

## disable copilot
因爲樹莓派處理比較慢,  copilot server 安裝大概需要好幾分鐘, 然後加上 seq 套件 default 的問題,  所以我們先把 copilot 的部分 mark 起來, 像下面這樣:

``` lisp
...
; copilot                                                                                          
;;(use-package copilot                                                                                
;;  :straight (:host github :repo "copilot-emacs/copilot.el" :files ("*.el"))                         
;;  :ensure t)                                                                                        
;; hook for completion                                                                              
;;(add-hook 'prog-mode-hook 'copilot-mode)                                                            
(defun my-copilot-tab ()                                                                            
  "If copilot completion is visible, accept by line. Otherwise, fallback to normal TAB."            
  (interactive)                                                                                     
  (if (copilot-current-completion)                                                                  
      (copilot-accept-completion-by-line)                                                           
    (indent-for-tab-command)))          
    
```

## reinstall 'seq' package 

 default **seq** 套件有開發分裂問題, 重新從 melpa 安裝 :
```lisp
M-x package-list-package 
;;找到 seq 套件, 用 'i' 指令重新安裝, 'x'指令執行

```
新的 seq 需要 restart emacs
### enable copilot

unmark 前面的 copilot 部分, 像這樣:
``` lisp
...
; copilot                                                                                          
(use-package copilot                                                                                
  :straight (:host github :repo "copilot-emacs/copilot.el" :files ("*.el"))                         
  :ensure t)                                                                                        
;; hook for completion                                                                              
(add-hook 'prog-mode-hook 'copilot-mode)                                                            
(defun my-copilot-tab ()                                                                            
  "If copilot completion is visible, accept by line. Otherwise, fallback to normal TAB."            
  (interactive)                                                                                     
  (if (copilot-current-completion)                                                                  
      (copilot-accept-completion-by-line)                                                           
    (indent-for-tab-command)))          
    
```

## install nodejs
在 bash 下安裝 nvm , 確定有 nodejs 可以用 : `nvm install 22.11.0`
確定 emacs 環境可以執行 nodejs (copilot server 需要)
## reintall copilot server

`M-x copilot-reinstall-server`

開 htop 來等, 等到 CPU useage 降下來,大概 3分鐘
然後就可以安裝好了

#emacs_install



