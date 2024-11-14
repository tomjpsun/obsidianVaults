# 取得 root permission 仍然無法設定
如果使用官方 .deb 安裝，可能遇到 `secure panel` 即使
詢問過 root 密碼，還是 disable 的情形。
這時候打指令 `anydesk-global-settings` 就可以進去設定。

+ 觀察

  ```
  ps aux | grep anydesk
  ```

  ```
  root        3992  0.0  0.1 367016 30200 ?        Ssl  08:23   0:00 /usr/bin/anydesk --service
  tom         8012  0.0  0.1 620720 30584 ?        Sl   08:37   0:00 /usr/bin/anydesk --tray
  tom        11386  0.0  0.0   6332  2056 pts/0    S+   09:37   0:00 grep anydesk
  ```

# ID 爲 0

網路查得到的說法是 Windows 防火牆沒有打開，無法從 Anydesk server 取得 ID。
但是在 Linux 下是怎麼回事？
原來是 AnyDesk 需要 Xorg 的環境，目前許多 Linux distribution 採用 wayland 並不支援，導致回報的問題並不是真正原因 。

![[螢幕快照 2024-02-21 09-01-57.png]]

重新登入，採用 Xorg 環境即可解決。
