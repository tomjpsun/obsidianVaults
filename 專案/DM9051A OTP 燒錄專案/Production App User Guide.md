
本文件是燒錄程式的手冊，介紹如何使用程式燒錄 Davicom DM 9051 A .

# Hardware Setup

以下將燒錄程式簡稱爲 App，本程式提供 Windows 與 Linux 版本。
App 在燒錄的 PC 上執行，首先，將燒錄器 USB 插入 PC，不需要安裝驅動程式，應該在系統裏面，分別會出現新的 COM 裝置 （Windows 下），或 ttyACM 裝置（Linux下），這是等一下要連接的界面。

# Start App

![[螢幕快照 2024-08-22 16-49-24.png]]

本版面顯示 App 的各項資訊，綠色標識是這個批次將要燒錄的 MAC 資訊:

> [!NOTE] 各種 MAC Address 說明
> Begin Mac: 批次燒錄的起始 MAC address（包含此位置）
> End Mac: 批次燒錄的結束 MAC address （包含此位置）
> Current Mac: 這是依照進度，需要燒錄的 MAC address
> Effect Mac: 讀取 Chip 目前的 Mac address ，可以比對 Current 看是否正確燒錄

`標識 1 `<span style="color: grey;">灰色</span>  代表目前沒有連接界面。
`標識 2` 將來選擇的連接界面，會顯示在此。

下一步：按  Configure Profile 按鈕`標識 3` 進入選擇設定。

# 設定燒錄參數

![[螢幕快照 2024-08-22 17-18-42.png]]

本頁面是一個 Dialog ， 我們可以在這裏設定批次燒錄的參數。

![[螢幕快照 2024-08-22 17-15-39.png]]

`COM Port` 這是下拉選單，選擇要透過那個界面連接燒錄器。
`Product ID`、`Vendor ID` 欲燒錄的 PID、VID

==燒錄迴圈== 是指燒錄後換裝 IC ，蓋上蓋子，程式自動監測已經蓋上後，自動燒錄下一個的迴圈動作，此時`標識 1` 的 State 會輪流在 <span style="color: orange;">Socket Wait</span>  與 <span style="color: red;">Burn</span> 切換：

| State       | Color on Tab | 說明                          |
| ----------- | ------------ | --------------------------- |
| Start       | 🟰           | App 開始，尚未與燒錄器連線             |
| Connected   | 🟩           | 已與燒錄器連線，尚未進入 Run Loop       |
| Socket Wait | 🟧           | 進入 Run Loop，Socket 等待 IC 放置 |
| Burn        | 🟥           | 燒錄中                         |

`Stop On Failure` : 表示如果燒錄失敗馬上跳出 ==燒錄迴圈==
`Skip Failed MAC` : 表示如果燒錄失敗，就繼續下一個 MAC 編號
`Overwrite Non Empty EEPROM` : 表示直接覆寫上去，不報任何錯誤。

設定好，按下 Finish 就開始燒錄，此時`標識 1` 的 State 會進入 <span style="color: green;">Connected </span>

![[Pasted image 20240827135952.png]]
# 開始燒錄

按下`Run`就進入 ==run loop==，此時`標識 1` 的 State 會進入 <span style="color: orange;">Socket Wait</span>，並開始燒錄， 過程中，`標識 1` 的 State 會進入  <span style="color: red;">Burn </span> 狀態，燒完後再回到 <span style="color: orange;">Socket Wait</span> 狀態。

Socket Wait:
![[螢幕快照 2024-08-27 14-02-53.png]]

Burning State:
![[Pasted image 20240827141751.png]]

批次燒錄結束後，會顯示停止的 dialog

![[Pasted image 20240827141946.png]]

此時，程式會斷開 COM 界面，以便於下次重新設定批次燒錄，所以畫面回到初始化的地方:

![[Pasted image 20240827145759.png]]

# Report

按下 Report 按鈕後，會得到 Summary Report，範例如下：

	REPORT	Version: 1.0.0
	PORT:	ttyACM0
	Time:
		FROM: 2024-08-27 13:56:11
		TO:   2024-08-27 14:17:44
		Elapsed: 1293 seconds
	MAC Range:
		Begin: 00:00:01:00:00:00
		End:   00:00:01:00:00:04
		Start: 00:00:01:00:00:00
	Failed MAC Addresses:
		None
	Summary:
		Range Count of MAC Addresses: 5
		Working Count of MAC Addresses: 5
		Failed MAC Addresses: 0
		Unfinished MAC Addresses: 0
		Success MAC Addresses: 5
		Success Ratio: 100%




==finished==

