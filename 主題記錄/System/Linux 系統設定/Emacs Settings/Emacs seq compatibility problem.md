`magit` 是 emacs 必裝套件，但是自從 `seq` v2.24 後就不相容，作者堅持不接受`seq` v2.24 的 default settings

這裏有升級操作，避免後續一直冒出warnings：
- M-x `package-refresh-contents`
- M-x `list-packages`
- /n `seq` to filter by name.
- i to mark a package for installation.
- x to execute your marked changes.

[Ref](https://emacs.stackexchange.com/questions/80871/how-to-provide-updated-seq-package-to-magit)

