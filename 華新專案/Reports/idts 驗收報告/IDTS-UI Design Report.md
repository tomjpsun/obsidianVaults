# 文件版本
|   |   |   
|---|---|
|Date|Ver.|
|2020-12-01|_1.0_

# 概述
本報告是關於 華新專案 Web UI 的說明，目前 release 版本爲 0.0.2-checkpoint-1-alpha，本程式是在  [[HMI 機器 ]] 上執行的 Web program, 目的在 :

1. 監控機臺周轉軸（電纜軸）的上、下架狀況
2. 系統的參數設定。

工廠類型和機台種類：

![[機台ID暫訂_1F.jpg]]

|   |   |   |   |   |   |   |   |   |
|---|---|---|---|---|---|---|---|---|
||Reader|給線機|給線滿軸準備區|給線空軸準備區|捲取機|捲取滿軸準備區|捲取空軸準備區|電梯|
|CS03 弓絞A|||||CS03T01|CS03T01RF01|CS03T01RE01||
|CS04 弓絞B|||||CS04T01|CS04T01RF01|CS04T01RE01||
|CS05 弓絞C|||||CS05T01|CS05T01RF01|CS05T01RE01||
|CS01 61B同心絞|||||CS01T01, CS01T02|CS01T01RF01, CS01T02RF01|CS01T01RE01, CS01T02RE01||
|BS01 1+4集合機||BS01G01, BS01G02, BS01G03, BS01G04, BS01G05|BS01G01RF01, BS01G02RF01, BS01G03RF01, BS01G04RF01, BS01G05RF01|BS01G01RE01, BS01G02RE01, BS01G03RE01, BS01G04RE01, BS01G05RE01|BS01T01|BS01T01RF01|BS01T01RE01||
|TP01 大包機||TP01G01|TP01G01RF01|TP01G01RE01|TP01T01|TP01T01RF01|TP01T01RE01||
|ST02 1600單扭機||   |||ST02T01|ST02T01RF01|ST02T01RE01||
|ST00||ST00G01~ST00G34|||||||
|ST01 1250單扭機|||||ST01T01|ST01T01RF01|ST01T01RE01||
|ST03 1600雙扭機||ST03G01, ST03G02, ST03G03, BST03G04, ST03G05|ST03G01RF01, ST03G02RF01, ST03G03RF01, ST03G04RF01, ST03G05RF01|ST03G01RE01, ST03G02RE01, ST03G03RE01, ST03G04RE01, ST03G05RE01|ST03T01|ST03T01RF01, ST03T01RF02|ST03T01RE01||
|EX01 Tandem A||EX01G01, EX01G02||EX01G02RE01|EX01T01, EX01T02|EX01T01RF01, EX01T02RF01|EX01T02RE01||
|EX02 Multipurpose A||EX02G01, EX02G02||EX02G02RE01|EX02T01, EX02T02||||
|EX03 CO A||EX03G01, EX03G02||EX03G02RE01|EX03T01, EX03T02||||
|EX04 CO B||EX04G01, EX04G02||EX04G02RE01|EX04T01, EX04T02||||
|EX05 Line A||EX05G01, EX05G02||EX05G01RE01, EX05G02RE01|EX05T01, EX05T02||||
|EX06 Line B||EX06G01, EX06G02||EX06G01RE01, EX06G02RE01|||||
|EX08 Line D||EX08G01, EX08G02|||EX08T01||||
|EX07 Line C||EX07G01, EX07G02||EX07G01RE01, EX07G02RE01|EX07T01, EX07T02||||
|EV11||||||||EV11F1|
|EV12||||||||EV12F1|

![[機台ID暫訂_3F.jpg]]


|天線|給線機|給線滿軸準備區|給線空軸準備區|捲取機|捲取滿軸準備區|捲取空軸準備區|電梯|
|-----|-----|-----|-----|-----|-----|-----|-----|
|TP02G01A, TP02T01A|TP02G01|TP02G01RF01|TP02G01RE01|TP02T01||||
|TP03G01A, TP03T01A|TP03G01|TP03G01RF01|TP03G01RE01|TP03T01||||
|TP04G01A, TP04T01A|TP04G01|TP04G01RF01|TP04G01RE01|TP04T01||||
|||||||||
|||||||||
|CS11G01A, CS11T01A|CS11G01|CS11G01RF01||CS11T01|CS11T01RF01|||
|CS12G01A, CS12T01A|CS11G01|CS11G01RF01||CS11T01|CS11T01RF01|||
|CS13G01A, CS13T01A|CS11G01|CS11G01RF01||CS11T01|CS11T01RF01|||
|CS06G01A,CS06G02A, CS06T01A|CS06G01, CS06G02|CS06G01RF01, CS06G02RF01|CS06G01RE01, CS06G02RE01|CS06T01|CS06T01RF01|CS06T01RE01||
|CS07G01A,CS07G02A, CS07T01A|CS07G01, CS07G02|CS07G01RF01, CS07G02RF01|CS07G01RE01, CS07G02RE01|CS07T01|CS07T01RF01|CS07T01RE01||
|CS08T01A||||CS08T01|CS08T01RF01|CS08T01RE01||
|CS09T01A||||CS09T01|CS09T01RF01|CS09T01RE01||
|CS10T01A||||CS10T01|CS10T01RF01|CS10T01RE01||
|DR03T01A, DR03T02A||||DR03T01, DR03T02||||
|||||||||
|||||||||
|EX12G01A, EX12G02A|EX12G01, EX12G02|||||||
|EX13G01A, EX13G02A|EX13G01, EX13G02|||||||
|EX10G01A|EX10G01|||EX10T01, EX10T02||||
|EX11G01A|EX11G01|||EX11T01, EX11T02||||

本程式根據不同的機臺，在每一臺 HMI 裝置上，提供相對應機臺之狀態監控，並且規劃分爲 三個開發階段，預計完成後，全部的頁面共有：

## 機臺設定
		![[Dashboard.png]]

這是第一階段預計的功能，也是 Dashboard 所在的頁面，根據 HMI configure 選定要觀察的機臺，然後畫面呈現目前觀察機臺的組態，包含：

##### 捲取段
###### 捲取機：
周轉軸安裝在此機器上，進行纜線捲取作業
###### 捲取空軸等待區：
附屬於捲取機，周轉軸放置區，準備安裝上捲取機
###### 捲取滿軸等待區。
附屬於捲取機，周轉軸放置區，從捲取機取下後放置於此

_以客戶術語描述，捲取段的周轉軸是 “滿下空上”_
##### 給線段
###### 給線機
周轉軸安裝在此機器上，進行纜線給線作業
###### 給線空軸等待區
附屬於給線機，周轉軸放置區，從給線機取下後放置於此
###### 給線滿軸等待區
附屬於給線機，周轉軸放置區，準備安裝上給線機

_以客戶術語描述，給線段的周轉軸是 “滿上空下”_

從前面表格可以知道：每種機臺的機器與等待區個數（即組態）不全部相同，但是同機種的組態會相同。

## Reader Status  

![[reader_status.png]]

這是第二階段功能，目前完成部分功能，可以 start/stop RFID Reader。


## HMI 設定
## 時間設定
## 備份設定

以上爲第二、三階段功能，目前尚未設計

#### NOTE： 第一階段展示 Dashboard，以觀察到周轉軸上、下架爲主要目的
    
# 用戶界面設計

我們以 NextJS 爲基礎架構，NextJS 是一個基於 React 的開源 JavaScript 框架，用於構建用戶端和服務端渲染的 Web 應用程序。它使構建和部署 Web 應用程序變得更加簡單高效。Next.js 提供了許多功能和優勢，使其成為當今最受歡迎的 React 框架之一。

我們的元件是採用 NextUI。NextUI 是一個現代化的 React UI 函式庫，專注於設計和易用性。它提供了一系列精心設計的組件，旨在創建美觀、響應迅速且易於使用的用戶界面。NextUI 的風格和特點使其在眾多 UI 函式庫中脫穎而出。
    
# 技術架構：

本程式使用 Javascript / Typescript 爲程式語言，架構以 NextJS 爲基礎，配合後端 Database services 與 Core services 來運作

![[idts_modules.png]]

其中 Core Services 模組與 DB Services 模組透過 API 提供顯示數據給 WebUI， 這兩個模組是基於 NodeJS 架構。
# 功能描述：
## 存取 HMI 參數
每臺 HMI 上線時，首先透過 Database 存取上次的參數，如果Database 沒有參數，例如首次執行時，則採用預設值。
## Dashboard 
根據 HMI 參數，向 Database Service 取得 HMI 欲監控之機臺組態，以 ST03 機臺爲例，Dashboard 必須顯示 捲取段：一個捲取機，其配置有一個空軸準備區，兩個滿軸準備區。給線段：五個給線機，各別配置有一個空軸準備區，一個滿軸準備區。如前表格所示，總共需要部署十九支天線監控。

因此，根據機臺組態，Dashboard layout 呈現左邊一組捲取設備 group，右邊五組給線設備 groups，每一個 group 都有機器與準備區。每一個機器或準備區都記錄下列訊息：

![[ReelData.png]]

+ 軸編號：周轉軸代碼
+ 製令：依照工單所給的代碼
+ Axis Code：將來提供客戶使用，目前沒有資料
+ 上架時間：周轉軸上架時間
+ 生產時間：周轉軸生產時間
+ 平均線速：周轉軸平均轉速
+ 備註：目前提供 Virtual Device 資訊，即 RFID Reader IP::天線編號

Dashboard 對於不同的機臺，會根據資料庫，找出附屬關係，並整理成 Axis groups
### Dashboard 元件

![[Dashboard_decomposite.png]]

紅色區域： 資料放在 **AxisData**  裏面， UI componet 是 **AxisCard** 
綠色區域： 資料放在 **ReelData** 裏面，UI component 是 **NextUI Card**
藍色區域：資料放在 **ReelStatus** 裏面，UI component 是 **ReelCard**

### Dashboard 流程

當 Dashboard 開始時，經由 **useEffect()** 的作用，會呼叫 **parseHMIConfig()**：

![[parseHMIConfig.png]]

之後也是 **useEffect()** 的作用，每兩秒更新 **buildDisplayData()**，就是向 Database Services 取得 production data 並更新到畫面：

![[buildDisplayData.png]]

所以 Dashboard 流程是：根據 HMI local 設定，提取本機 HMI 的 configure data，建立監控機臺的樹狀結構，呈現在畫面上，然後定時（每2秒）讀取 production data，根據 production data 來更新畫面，反應機臺上、下架的狀態。
# 性能和優化：
採用 NextJS 框架，將渲染部分拆成可以在框架的 Server 執行，就預先 render 頁面，再送到框架的 FrontEnd 執行，其他就等到 FrontEnd 進行 DOM tree rendering，所以效能可以事先優化。
# 安全性和可靠性**：
    
由 SWR library 提供 fetch 行爲，如果 fetch 失敗
# 測試和質量保證**：
    
    - 測試策略和框架。
    - 主要測試用例和測試結果。
    - Bug 跟蹤和修復記錄。
# 部署和維護**：
    
    - 部署流程和環境。
    - 持續集成和持續部署（CI/CD）策略。
    - 維護計劃和更新策略。
# 用戶培訓和文件**：
    
    - 用戶手冊和操作指南。
    - 培訓材料和視頻。
    - 常見問題解答（FAQ）。
# 項目管理**：
    
    - 項目團隊和角色分配。
    - 開發時間線和里程碑。
    - 預算和資源管理。
# 用戶反饋和改進**：
    
    - 用戶測試反饋。
    - 改進計劃和迭代策略。
# 附錄**：
    
    - 參考文獻和資源。
    - 第三方庫和工具的許可。
    - 代碼片段或額外的技術細節。