
# 燒錄裝置架構規格

## 系統組成
由以下幾個模組組成：
#### MCU 模組：
負責控制燒錄流程，包括接收 GUI 的 Profile 指令、燒錄、回報燒錄結果等。
Altery 型號  AT32F403AVGT7

硬體示意圖：
![[DM9051A-OTP-Programmer-Controller.png]]
軟體示意圖：
![[軟體示意圖-1.png]]

![[軟體示意圖-2.png]]
#### SOCKET 模組：
由 MCU 控制，界面爲 SPI，負責將燒錄內容輸出到 8051A Embedded Memory(One Time Programmed Device => OTP)
#### USB Serial 模組：
負責 MCU 與 GUI 通訊，將 Serial commands 包在 USB 裏面傳輸， Serial commands 爲 USER 對 燒錄器的命令
#### GUI / HMI：
本產品目前定位在 PC/Windows 上使用，有 GUI 操控畫面，透過 Serial commands 對燒錄器下命令。

# 系統功能描述
## 燒錄器
燒錄裝置燒錄一次性的記憶裝置(OTP)
### 接收 Profile
燒錄裝置可以接收來自 GUI 的 Profile 指令。Profile 指令包括燒錄的內容、燒錄的起始地址、燒錄的數量等。
### 回報燒錄結果
燒錄裝置在燒錄每個 IC 完成後，會向 GUI 回報燒錄結果。燒錄結果包括燒錄的地址、燒錄的狀態等。User 也可以回看記錄。


# 傳輸界面
## MCU 模組
支持 SPI 通信。
支持 RS-232 通信。
具有足夠的存儲空間來存儲燒錄的內容。MCU 內存容量 256ZW/768NZW, total 1024KB
## SPI 模組
SPI 模組採用常用的 SPI 接口。
MCU 透過 SPI 連接 SOCKET 燒錄。
SPI 速度規格 Max：50 Mbit/s depends on DM9051A
## USB Serial 模組
MCU 透過 Serial Interface 轉接到 USB Serial 界面，連接到 HMI 裝置
界面支援 Baud Rate 115200

### Programmer Protocol With HMI

|意義|代號|值|Size|備註|
|---|---|---|---|---|
|Start Frame|STX|02H|1||
|Length Low|||1|From STX~ETX exclude STX|
|Length High|||1|From STX~ETX exclude STX|
|CMD|OP|See OP Table|1||
|Data|DATA|See OP Table||MAX 2048|
|Stop Frame|ETX|03H|1||
|CRC Low|||1|From STX~ETX exclude STX|
|CRC High|||1|From STX~ETX exclude STX|
|意義|代號|值|Size|備註|

### OP Table

|意義|值|Size|備註|
|---|---|---|---|
|Profile Write|55H|1|Write Profile Data Format|
|Response OP|A5H|1||

### Write Profile Data Format
|意義|值|Size|備註|
|---|---|---|---|
|Offset ||1|目前僅支援值爲 0 (從頭寫）|
|Data Length ||1|目前僅支援值爲 18 (9051 Profile Length)|
|Data  ||||


# 系統測試
GUI 提供界面，以驗證下列功能：
## 驗證燒錄硬體設備
+ 驗證燒錄裝置的 SPI 接口和 RS-232 接口能夠正常工作
+ 驗證燈號可以正常操作。
## 驗證燒錄功能正確
+ 能夠按照 Profile 指令正確燒錄 ：最基本的是：寫完後讀回核對內容。

# 系統擴展（下階段）
燒錄裝置的改進方向包括：
支持量產使用


# 附錄 A：8051A OTP GUI 
![[DM 9051A OTP Programmer GUI Spec.pdf]]