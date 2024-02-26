
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
## Template：
等同 **出廠設定** ， 包括 要燒錄的 MAC Address 範圍，這個範圍由 MAC begin 到 MAC end 指定，還有 PID 、VID、Template 版號。
Template  存檔資訊附帶 MD5 ，方便確認爲原廠 Template。

## Profile：
量產燒錄的命令，從 Template 取得初始內容，**外加**
Source Template 版本號碼、目前 COM Port 、目前燒錄的 MAC Address


