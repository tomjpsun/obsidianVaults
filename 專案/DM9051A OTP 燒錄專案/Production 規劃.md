
```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```


# 定義
下列定義都是以 JSON format 存檔，方便使用者改寫。 
## Settings：
程式初始需要讀的設定，包括 Source Template，current profile x 4， 程式版號、Template 版號。
## Template & Profile x 4
當按下 **New Programmer** 後，從 Template copy 一份 Profile， UI 出現填寫欄位，讓 user 填寫內容：
### 出廠設定：
包括 要燒錄的 MAC Address 範圍，這個範圍由 MAC begin 到 MAC end 指定，還有 PID 、VID、Template 版號。
### run time 設定的初始值：
在 Profile 產生的時候，給予初始值，有：
Source Template 檔案名稱、目前燒錄的 MAC Address，Profile 產生日期。
另外產生 Profile 的時候，要求使用者選擇 COM Port 並保存在 Profile 內。

>[!MD5]
Template  存檔資訊附帶 MD5 ，方便確認爲原廠 Template。


