如果 Login 後, Gnome 系統設定可以設定 sleep, 但是當 user logout 後, 需要透過系統等級的設定, 如下:
```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target
```

如果要恢復:

```bash
sudo systemctl unmask sleep.target suspend.target hibernate.target hybrid-sleep.target
```

\#
