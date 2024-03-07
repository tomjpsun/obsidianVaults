
| 本文件版本 | comment           |     |
| ----- | ----------------- | --- |
| 0.1.0 | 初稿                |     |
| 0.1.1 | Review 2024/02/27 |     |

# 定義
下列定義都是以 JSON format 存檔，方便使用者改寫。 
## Settings：

程式初始需要讀的設定，第一次自行 create。
欄位如下：

| Field                | Example Value                      | Initial Value                                                    |
| -------------------- | ---------------------------------- | ---------------------------------------------------------------- |
| Source Template File | ~/my_folder/templates/DM9051A.tmpl | ${HOME}/programmer/templates/DM9051A.tmpl (create if not exist ) |
| Profile 1            | my_1.json                          | profile_1.json                                                   |
| Profile 2            | test_2.json                        | profile_2.json                                                   |
| Profile 3<br>        | ex3.json                           | profile_3.json                                                   |
| Profile 4            | 4.json                             | profile_4.json                                                   |
| Profile Path         | ~/my_folder/profiles/              | get from QFileDialog, default to ${HOME}/programmer/profiles/    |
| Template Version     | 1.0.0                              | 1.0.0                                                            |
| App Version          | 1.0.0                              | 1.0.0                                                            |
| Log Path             | ~/my_folder/logs/                  | ${HOME}/programmer/logs/                                         |
| Report Path          | ~/my_folder/reports/               | ${HOME}/programmer/reports/                                      |

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
| Source Template File |                   | Settings[`"Source Template File"`] |     |
| ManuFacturer         | DAVICOM           | DAVICOM                            |     |
| StopOnFailure        | Y                 | Y                                  |     |
| SkipFailedMAC        | N                 | N                                  |     |

>[!備註]
>Source Template File 從 Setting 讀取
>Report Path：產生 Report 時存放的檔案Path

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
| Current MAC Address       | AA:BB:CC:DD:EE:00         | Settings [`"Begin Mac Address"`] |
| Profile Number(one start) | 1                         | 1                                |
| COM Port                  | com 5                     | NULL                             |
| Log File                  | 2024_0226_160530_com5.log | 依照 create time 初始化               |


>[!Note]
>Log File 每次 run 的時候，用當時時間產生一個，檔案名稱格式爲 {year}\_{month}{day}\_{hour}{min}{sec}\_{COM}.log

# 流程

MainWindow ->讀取 App Settings
新增 Programmer -> Wizard 
Wizard 步騶:
###### 頁面 1
+ connect programmer ，讀取 programmer ID，
+ connect 成功才有下一步
###### 頁面 2
+ 取得 user 放 template 位置，並將 default template 放在該位置
+ 取得 user 放 profile 位置，並將 profile 放在該位置
+ 成功寫入 file 才有下一步
###### 頁面 3
+ 從 template 取得 profile 初始值，給 user 修改，修改後更新上面的 profile 內容

>[!Info] 
>如果 Wizard cancel 則 disconnect programmer
>如果都完成就在 MainWindow 新增頁面

>[!Warning] 
>每一個頁面每秒向燒錄器讀取 status ，如果超過 3 次沒有回應，就跳出斷線的 dialog 然後移除本頁面


![[螢幕快照 2024-03-07 16-08-02.png]]
