剛裝完 Debian 12 遇到的，以前沒有遇過
![[Screenshot from 2023-09-21 17-24-45 2.png]]
主機有支援兩個網卡，但是 enable 的時候，會互斥（i.e. 另一個被 gnome disable）
這時候按`+` 符號，新增 profile：
![[Screenshot from 2023-09-21 17-23-43.png]]
profile 是不會互斥的，最後像這樣，兩個網卡都可以用

![[Screenshot from 2023-09-21 17-23-20.png]]
記得點選 profile 才會 enable ！
