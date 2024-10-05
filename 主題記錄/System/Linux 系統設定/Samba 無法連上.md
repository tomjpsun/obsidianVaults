#### 解決辦法
在 server 端，確定 `sudo smbpasswd -a tom` 將連線的 user 加入， 這範例的 user 是 tom。

已經有跑 service `firewalld`，用工具 `firewall-config` 將 `samba` & `samba-client` 打開

關閉後從 client 端重連，可以了。
