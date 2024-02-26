# 定義
下列定義都是以 JSON format 存檔，方便使用者改寫。 
## Settings：

程式初始需要讀的設定，第一次自行 create。
欄位如下：

| Field                | Example Value  | Initial Value                         |
| -------------------- | -------------- | ------------------------------------- |
| Source Template File | ./DM9051A.tmpl | ~/DM9051A.tmpl (create if not exist ) |
| Profile 1            | my_1.json      | profile_1.json                        |
| Profile 2            | test_2.json    | profile_2.json                        |
| Profile 3<br>        | ex3.json       | profile_3.json                        |
| Profile 4            | 4.json         | profile_4.json                        |
| Profile Path         | ~/Profile      | get from QFileDialog                  |
| Template Version     | 1.0.0          | 1.0.0                                 |
| App Version          | 1.0.0          | 1.0.0                                 |
| Log Path             | ~/Log          | ~/Log                                 |

## Template
當按下 **New Programmer** 後，從 Template 複製 一份 Profile， UI 出現填寫欄位，讓 user 填寫內容：
### 出廠設定：
包括 要燒錄的 MAC Address 範圍，這個範圍由 MAC begin 到 MAC end 指定，還有 PID 、VID、Template 版號。


| Field                | Example           | Initial Value                      |     |
| -------------------- | ----------------- | ---------------------------------- | --- |
| Begin MAC Address    | AA:BB:CC:DD:EE:00 | 00:00:00:00:00:00                  |     |
| End MAC Address      | AA:BB:CC:DD:EE:FF | 00:00:00:00:00:00                  |     |
| PID                  | 9051              | 9051                               |     |
| VID                  | 0A46              | 0A46                               |     |
| Template Version     | 1.0.0             | 1.0.0                              |     |
| Programmer Device ID | DM_PROG_01        | DM_PROG                            |     |
| Source Template File |                   | Settings[`"Source Template File"`] |     |

>[!備註]
>Programmer Device ID 先留位置，以後再從 API 讀取
>Source Template File 從 Setting 讀取

>[!MD5]
Template  存檔資訊附帶 MD5 ，方便確認爲原廠 Template。


## Profile
當 User 按下 New Programmer 之後，經過 Wizard 詢問 User 後，新增一個 Tab 顯示新的 Profile 內容。
Wizard 根據 Template 內容對新增的 Profile 初始化，並且額外新增 Runtime Values
### run time 設定的初始值：
在 Profile 產生的時候，給予初始值，有：
Source Template 檔案名稱、目前燒錄的 MAC Address，Profile 產生日期。
另外產生 Profile 的時候，要求使用者選擇 COM Port 並保存在 Profile 內。

>[!Note]
>