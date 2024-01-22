問題描述：使用 Qt Creator 編輯 UI 的時候，發現 包在 **Frame Widget** 的 UI 元件，會有內定的 **LayoutLeftMargin** , **LayoutRightMargin**, 非零，這樣跟其他 Container 如 **GroupBox** 的對齊邊緣不一致 （**GroupBox** 沒有

![[螢幕快照 2024-01-22 10-01-28.png]]

最後設定外包的 layout margin 即可。

![[螢幕快照 2024-01-22 10-18-42.png]]