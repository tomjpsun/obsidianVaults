公司的設定需要**機器直接連大網**，不能透過 Router

Xerox 官方網站沒有給 Linux 的驅動，先安裝 Debian 的驅動：

	$> sudo apt install printer-driver-fujixerox

然後用 [CUPS 安裝](http://localhost:631/admin)
按下 `Add Printer` 
![[Pasted image 20240829154608.png]]

然後選 MAC address 尾數 54:7c:eb ，型號 FUJI XEROX ApeosPort-IV 3065 ，網址 192.168.1.68

`continue` 後，Location 隨便打，這裏我打 7F：

![[Pasted image 20240829155022.png]]

`continue` 後選擇 FUJI XEROX：
![[Pasted image 20240829155102.png]]

`add printer` 後，選擇 Fuji Xerox PDF Printer

![[Pasted image 20240829155217.png]]
`Add Printer` 之後應該就可以用了
