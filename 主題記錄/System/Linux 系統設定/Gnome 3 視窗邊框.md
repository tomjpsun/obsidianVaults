目前沒有邊框，看起來像這樣：
![[Pasted image 20240411161019.png]]

設定邊框後，用起來清楚多了：
![[螢幕快照 2024-04-11 09-23-17.png]]

如何修改呢？ 打開 **~/.config/gtk-3.0/gtk.css**
如果沒有就 create 一個，內容如下：

```css
/* ~/.config/gtk-3.0/gtk.css */

decoration {
	background-color: inherit;
	border-radius: 8px;
	border-bottom-left-radius: inherit;
	border-bottom-right-radius: inherit;
	border: 1px solid rgb(144,144,144);
	border-top: none;
}

```

改完後重新登入使得 Gnome/gtk 重新載入即可。