
| 本文件版本 | comment                             |     |
| ----- | ----------------------------------- | --- |
| 0.1.0 | 初稿                                  |     |
| 0.1.1 | Review 2024/02/27                   |     |
| 0.1.2 | 2024/3/12 新增流程章節，修改 Settings ，初稿待討論 |     |

# 定義
下列定義都是以 JSON format 存檔，方便使用者改寫。 
## Settings：

程式初始需要讀的設定，第一次自行 create。
欄位如下：

| Field                | Example Value                      | Initial Value                                                    |
| -------------------- | ---------------------------------- | ---------------------------------------------------------------- |
| Source Template File | ~/my_folder/templates/DM9051A.tmpl | ${HOME}/programmer/templates/DM9051A.tmpl (create if not exist ) |
| Profile\[1]          | my_1.json                          | profile_1.json                                                   |
| Profile\[2]          | test_2.json                        | profile_2.json                                                   |
| Profile\[3]<br>      | ex3.json                           | profile_3.json                                                   |
| Profile\[4]          | 4.json                             | profile_4.json                                                   |
| Profile Path         | ~/my_folder/profiles/              | get from QFileDialog, default to ${HOME}/programmer/profiles/    |
| Template Version     | 1.0.0                              | 1.0.0                                                            |
| App Version          | 1.0.0                              | 1.0.0                                                            |
| Log Path             | ~/my_folder/logs/                  | ${HOME}/programmer/logs/                                         |
| Report Path          | ~/my_folder/reports/               | ${HOME}/programmer/reports/                                      |

## Template
當按下 **New Programmer** 後，從 Template 複製 一份 Profile， UI 出現填寫欄位，讓 user 填寫內容：
### 出廠設定：
包括 要燒錄的 MAC Address 範圍，這個範圍由 MAC begin 到 MAC end 指定，還有 PID 、VID、Template 版號。


| Field                 | Example                            | Initial Value                      |     |
| --------------------- | ---------------------------------- | ---------------------------------- | --- |
| Begin MAC Address     | AA:BB:CC:DD:EE:00                  | 00:00:00:00:00:00                  |     |
| End MAC Address       | AA:BB:CC:DD:EE:FF                  | 00:00:00:00:00:00                  |     |
| PID                   | 9051                               | 9051                               |     |
| VID                   | 0A46                               | 0A46                               |     |
| Template Version      | 1.0.0                              | 1.0.0                              |     |
| Source Template File  | ~/my_folder/templates/DM9051A.tmpl | Settings[`"Source Template File"`] |     |
| ManuFacturer          | DAVICOM                            | DAVICOM                            |     |
| StopOnFailure         | Y                                  | Y                                  |     |
| SkipFailedMAC         | N                                  | N                                  |     |
| Refresh Interval\(ms) | 1000                               | 1000                               |     |

>[!備註]
>Source Template File 從 Setting 讀取
>Report Path：產生 Report 時存放的檔案Path
>Refresh Interval 以 millisec 爲單位，不提供使用者調整

>[!MD5]
Template  存檔資訊附帶 MD5 ，方便確認爲原廠 Template。


## Profile

當 User 按下 New Programmer 之後，經過 Wizard 詢問 User 後，Wizard 根據 Template 內容對新增的 Profile 初始化，UI 新增一個 Tab 顯示其內容。
### Static 設定的初始值：
參考[Template 出廠設定](#Template)
### Run time 設定的初始值：

在 Profile 產生的時候，給予初始值，除了 Template 的欄位之外，Runtime 欄位如下：

| Field                     | Example                   | Initial Value                    |
| ------------------------- | ------------------------- | -------------------------------- |
| Current MAC Address       | AA:BB:CC:DD:EE:00         | copy from `Begin MAC Address`    |
| Profile Number(one start) | 1                         | index of profile                 |
| COM Port                  | com 5                     | NULL                             |
| Log File                  | 2024_0226_160530_com5.log | 依照 create time 初始化, 檔名參考下面的 Note |
| Refresh Interval\(ms)     | 1000                      | 1000                             |


>[!Note]
>Log File 每次 run 的時候，用當時時間產生一個，檔案名稱格式爲 {year}\_{month}{day}\_{hour}{min}{sec}\_{COM}.log, 例如 2024_0312_103053_COM5.log。
>Refresh Interval 以 millisec 爲單位，不提供使用者調整。

# 流程設計
## App Start
App start 時，讀取 **settings**（初始從 resource 來）， 進入 `configure settings wizard`，從 template 取得初始值，提供 user 修改 幾個 paths：
+ source template path
+ profile path
+ log path
+ report path
結束後存入 **settings** 。
App 有 `configure settings` button ，讀取 settings 的值，讓 user 重新 configure。
結果存回 **settings**

App 畫面 4 個 Tabs，初始不連上 Programmer，之後自動 connect
各自有各自的 profile

>[!Note] 在 `configure settings` 的過程，關於每個 `profile`  設定，不進行設定，
以後每個 tab 裏面提供按鈕呼叫 `configure profile wizard` 

>[!Info] 
>第一階段提供一個 tab，第二階段提供 4 個 tabs
## Tab i 頁面 （ i 是頁面 index 從 0 開始)

### Tab i Wizard

提供 `configure profile` button ，進入 `configure profile wizard` 提供 user 修改 ：
+ 讀取 settings.profile[i] 的檔案內容，給 user 修改
+ 結果存回 settings.profile[i] 的檔案
+ wizard 頁面提供下列設定：
	+ Start MAC Addr
	+ End MAC Addr
	+ PID
	+ VID
	+ Stop on error （討論：每次都會打開蓋子，是否需要這個設定？）
	
### Tab i 

顯示下列內容：
+ Start MAC Addr
+ End Mac Addr
+ Current Mac Addr
+ 用 icon 分別表示 蓋子打開、關閉
+ a `configure profile` button
+ polling state 依照 refresh_interval\(ms)
+ Tab color for state : Table 1

| State | Color  |     |
| ----- | ------ | --- |
| 正在燒錄  | Orange |     |
| 燒錄成功  | Green  |     |
| 燒錄失敗  | Red    |     |
**Table 1**





