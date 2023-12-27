## 如果使用官方 .deb 安裝，可能遇到 `secure panel` 即使
詢問過 root 密碼，還是 disable 的情形。
這時候打指令 `anydesk-global-settings` 就可以進去設定。

## 觀察

-
  ```
  ps aux | grep anydesk
  ```


-
  ```
  root        3992  0.0  0.1 367016 30200 ?        Ssl  08:23   0:00 /usr/bin/anydesk --service
  tom         8012  0.0  0.1 620720 30584 ?        Sl   08:37   0:00 /usr/bin/anydesk --tray
  tom        11386  0.0  0.0   6332  2056 pts/0    S+   09:37   0:00 grep anydesk
  ```