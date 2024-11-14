
# 首頁

## 1. 簡介
本規格書描述了一個網頁頁面的功能，該頁面主要用於監控一組 RFID Readers。每個 Reader 都有“設定”與“狀態”兩大部分需要陳述。

## 2. 功能描述

### 2.1 設定部分

#### 2.1.1 啓動/停止
- 提供“啓動”與“停止”兩個 buttons，用於控制 RFID Reader 的運作。

#### 2.1.2 sync NTP
- 提供“sync NTP” button，用於對 NTP 進行時間同步。
- 同步的動作會根據 configure 參數來執行。

#### 2.1.3 configure
- 提供“configure” button。
- 點擊後會跳轉到另一個頁面，用於輸入 configure 參數給 reader。

### 2.2 監控部分

#### 2.2.1 Reader 系統溫度
- 在頁面上顯示 RFID Reader 的系統溫度。

#### 2.2.2 天線溫度
- 在頁面上顯示 RFID Reader 的天線溫度。

#### 2.2.3 PA 溫度
- 在頁面上顯示 RFID Reader 的 PA 溫度。

#### 2.2.4 Reader IP/Port/Link status
- 在頁面上顯示 RFID Reader 的 IP、Port 以及 Link 的狀態。

#### 2.2.5 天線狀態
- 在頁面上顯示 4 根天線的狀態。

## 3. 使用者介面

### 3.1 設定部分
- 顯示“啓動”、“停止”、“sync NTP”以及“configure”四個 buttons。
- 每個 button 旁邊應有簡單的文字說明其功能。

### 3.2 監控部分
- 使用表格或卡片形式顯示每個 RFID Reader 的狀態。
- 每個 Reader 的狀態應包括：系統溫度、天線溫度、PA 溫度、IP、Port、Link status 以及 4 根天線的狀態。

## 4. 其他需求
- 頁面應該具有自動刷新功能，以實時顯示 RFID Reader 的最新狀態。
- 頁面應該具有使用者認證功能，以確保只有授權的使用者可以訪問。

## 5. 結論
本規格書描述了 RFID Reader 監控頁面的主要功能和需求，為開發者提供了清晰的指引。
