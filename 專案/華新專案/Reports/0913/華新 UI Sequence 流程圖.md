#### Dashboard

```mermaid

sequenceDiagram
  participant Dashboard
  participant DB
  Dashboard->> HMI:讀取本機 HMI ID
  Dashboard->> DB: 取得 HMI 資料
  DB-->>Dashboard: 回傳 HMI 資料
  Dashboard->>DB: 取得機臺設定與組態
  DB-->>Dashboard: 回傳機臺設定與組態

  loop Motitor
	  Dashboard->>DB: 取得機臺資料
	  DB-->>Dashboard: 回傳機臺資料
	  Dashboard ->>Dashboard: period
  end


```


#### 機臺設定

```mermaid
sequenceDiagram
  participant UI
  participant DB

  UI->> HMI:讀取本機 HMI 控管的機臺編號
  HMI-->>UI:回傳
  UI->> DB: 取得機臺設定
  DB-->>UI: 回傳設定
  loop 更改
	  UI->>DB: 更新機臺資料
	  DB-->>UI: 讀取最新機臺資料
	  UI ->>UI: period
  end
```


#### 系統設定

匯入：

```mermaid
sequenceDiagram
  participant UI
  participant DB

  UI->> HMI:讀取本機 settings file
  HMI -->>UI:回傳 settings
  UI->> DB: 將 settings 寫入 DB
  DB-->>UI:回傳成功或失敗

```


修改：

```mermaid
sequenceDiagram
  participant UI
  participant DB

  UI->> DB: 讀取 settings
  loop
	  UI ->>UI: 更新欄位
  end
  UI->> DB: 寫入 settings
  DB->>HMI: 採用新的設定

```

主控臺

```mermaid
sequenceDiagram
  participant UI
  participant DB

  UI->> DB: start/stop service command
  loop
	  DB ->>DB: 等待更新
  end
  DB-->> UI: result

```


#### Report
```mermaid
sequenceDiagram
  participant UI
  participant DB

  UI->> DB: get (Day/Week/Month) report
  DB-->> UI: result
  ```
