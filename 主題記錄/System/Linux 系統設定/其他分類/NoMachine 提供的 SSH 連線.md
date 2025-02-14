如果連上遠端 NxServer 後, 想要類似 VLAN 的連線,可以這麼做:

右上角翻下來的部分, 用滑鼠點選:
[[主題記錄/System/Linux 系統設定/其他分類/attachments/58de70af917562a4cde7b09b2db9e57a_MD5.jpeg|Open: Pasted image 20250213095822.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/58de70af917562a4cde7b09b2db9e57a_MD5.jpeg]]
選 `Devices`
[[主題記錄/System/Linux 系統設定/其他分類/attachments/277cc9b78a21bac24390cb76a68a8f53_MD5.jpeg|Open: Pasted image 20250213095959.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/277cc9b78a21bac24390cb76a68a8f53_MD5.jpeg]]
選 `Connect a Network Port`
[[主題記錄/System/Linux 系統設定/其他分類/attachments/3c1697c2032a5f7cddf0e8da602dbf9c_MD5.jpeg|Open: Pasted image 20250213100032.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/3c1697c2032a5f7cddf0e8da602dbf9c_MD5.jpeg]]

`Remote Ports` 裏面選 `Add a new port`

[[主題記錄/System/Linux 系統設定/其他分類/attachments/a62b148c2b6e1b8c1d92437c17669970_MD5.jpeg|Open: Pasted image 20250213100209.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/a62b148c2b6e1b8c1d92437c17669970_MD5.jpeg]]
Service 選 ssh , 其他都用 default , 然後 connect
[[主題記錄/System/Linux 系統設定/其他分類/attachments/379ea0294c231271f18c34165933bdd8_MD5.jpeg|Open: Pasted image 20250213100339.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/379ea0294c231271f18c34165933bdd8_MD5.jpeg]]

最後看到 SSH 可以透過 VLAN 連線
[[主題記錄/System/Linux 系統設定/其他分類/attachments/d7bf8b83e52a1bbea6880d845beb38f2_MD5.jpeg|Open: Pasted image 20250213100426.png]]
![[主題記錄/System/Linux 系統設定/其他分類/attachments/d7bf8b83e52a1bbea6880d845beb38f2_MD5.jpeg]]

打開本機 console 以這個爲例子, 連線命令:
```sh
ssh 169.254.127.6
```

