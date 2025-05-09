### **工溫測試系統流程說明 (Central Bench 流程解析)**

本說明整理 `central_bench.py` 的運行流程，幫助使用者理解該程式如何執行測試、收集數據、存儲結果並產生報告。

---

# **1. 初始化 BenchConfig 設定**

---

## **1.1 載入 `bench.cfg` 設定檔**

```python
if not BenchConfig.load_config("bench.cfg"):
    print("Cannot load bench configure file: bench.cfg")
    exit(0)
```

- **功能**：讀取 `bench.cfg` 設定檔，載入測試環境參數，如 `interface`、`n_loop`、`epoch` 等。
---
- **關鍵參數**
    - `interface`: 測試網路介面
    - `epoch`: 測試回合數
    - `n_loop`: 每回合測試次數
    - `valid_mac_begin` & `valid_mac_end`: 合法的 MAC 位址範圍
    - `database_dir`: 測試數據儲存目錄

---

# **2. 建立並啟動資料庫**

---

## **2.1 設定資料庫名稱**

```python
timestamp = datetime.now().strftime("%Y%m%d")
db_name = BenchConfig.database_dir + "/bench-" + timestamp + '.db'
print(f"Database: {db_name}")
```

- **功能**：根據日期命名 SQLite 資料庫，例如 `/tmp/bench-20240320.db`。

---
## **2.2 啟動資料庫儲存執行緒**

```python
db_quit = threading.Event()
db_thread = threading.Thread(target=prepare_db, args=(db_name, db_queue, db_quit,))
db_thread.start()
```

- **功能**：開啟獨立執行緒，透過 `prepare_db()` 持續寫入測試數據至資料庫。

---

# **3. 開始測試**

---

### **3.1 標記測試開始**

```python
dprint(DPrintSummary, 0, 0, DPrintSummaryBegin, f"Bench Start")
bench_start_print()
```

- **功能**：記錄 **測試開始時間**，並印出測試參數。

---
### **3.2 掃描可用目標設備 (Polling)**

```python
resp = init_module()
```

- **功能**：發送 `PollingCommand`，收集可測試的目標設備 (TA, Test Article)。
- **成功條件**：至少發現一個可測試設備，否則測試終止。

---
### **3.3 驗證 MAC 位址**

```python
if not validate_mac_range(resp):
    print("MAC out of range, please check")
    exit(0)

if not validate_mac_overlap(resp):
    print(f"MAC address overlapped: {mac_values}")
    exit(0)
```

- **功能**：確保回應的設備 MAC 位址在 `bench.cfg` 指定的範圍內，且不重複。

---

# **4. 啟動封包監聽**

```python
t = AsyncSniffer(iface=BenchConfig.interface,
                 prn=packet_callback,
                 filter=f"ether proto 0x0901 or ether proto 0x0D01 or ether proto 0x0B01",
                 store=0)
t.start()
```

- **功能**：開啟異步封包監聽 (Sniffer)，捕捉來自設備的回應封包。
- **監聽範圍**
    - `0x0901` (Polling 回應)
    - `0x0D01` (RX Command 回應)
    - `0x0B01` (PHY Command 回應)

---

# **5. 測試執行 (Bench Target)**

---

## **5.1 為每個設備啟動測試執行緒**

```python
bench_all(epoch=BenchConfig.epoch, n_loop=BenchConfig.n_loop)
```

- **功能**：根據設備數量啟動多個執行緒，每個設備由一個執行緒進行測試。
---
## 5.2 單個設備的測試流程

```python
def bench_target(epoch, n_loop, thread_index):
```
### 流程

#### 標記設備資訊
        ```python
        dprint(DPrintReport, 0, 0, 
        thread_index, f"Target:{thread_index}, 
        ID:{BenchConfig.dst_mac[thread_index]}")
        ```

---
#### 執行不同模式測試
        
        ```python
        if BenchConfig.test_modes["100Full"]:
            if change_mode(0xff, [rx_cmd, phy_cmd]):
                loop_test(n_loop, [rx_cmd, phy_cmd])
        ```

---    
#### 記錄測試結果
        
        ```python
        dprint(DPrintSummary, 0, 0, DPrintNormal, f"Target {thread_index} RX count = {rx_cmd.sequence:04x}")
        ```

---

## **6. 測試結束與報告產生**

---

### **6.1 停止封包監聽**

```python
t.stop()
t.join()
```

- **功能**：停止 Sniffer，避免影響後續報告產生。
---
### **6.2 標記測試結束**

```python
dprint(DPrintSummary, 0, 0, DPrintSummaryEnd, f"Bench Finished")
```

- **功能**：記錄測試結束時間。
---
### **6.3 產生測試報告**

```python
db = DatabaseManager(db_name)
db.connect()
print_summary(db)
db.close()
```

- **功能**：讀取資料庫數據並輸出測試報告，包含：
    - 測試開始與結束時間
    - 各設備的測試結果
    - RX 逾時錯誤統計
    - PHY 錯誤統計

---

## **7. 結束測試**

```python
db_quit.set()
db_thread.join()
```

- **功能**：通知資料庫執行緒關閉，確保所有測試數據已存入 SQLite。

---

## 總結

| 步驟            | 功能                     |
| ------------- | ---------------------- |
| **1. 載入設定**   | 讀取 `bench.cfg`         |
| **2. 啟動資料庫**  | 建立 SQLite 檔案，啟動數據儲存執行緒 |
| **3. 開始測試**   | 標記開始時間，執行 `Polling`    |
| **4. 啟動封包監聽** | 開啟 Sniffer 監聽封包        |


---

| **5. 測試執行**      | 依 `epoch` & `n_loop` 測試設備 |
| ---------------- | ------------------------- |
| **6. 測試結束與報告產生** | 停止測試，產生 `summary`         |
| **7. 關閉資料庫**     | 結束數據存儲執行緒                 |

---


以下是 `central_bench.py` 流程圖：


https://drive.google.com/file/d/1eNr7qBgSyISSRTYyCnU8NK3fh4rp3eac/view?usp=sharing




---
📌 **這張圖的流程解釋：**

1. **初始化**
    
    - 讀取 `bench.cfg` 設定
    - 建立 SQLite 資料庫
    - 啟動資料庫執行緒，開始測試記錄
2. **Polling**
    
    - 發送 `PollingCommand` 尋找設備
    - 驗證 MAC 位址是否合法
    
---
3. **啟動測試**
    
    - 啟動 **多個執行緒** 針對不同設備執行測試
    - 根據設定執行 100 Full、100 Half、10 Full、10 Half 測試
    - 記錄 RX 結果
4. **結束測試**
    
    - 關閉封包監聽
    - 產生 `summary` 測試報告
    - 停止資料庫執行緒，關閉 SQLite
---
## 8.Report

某一次測試 Report 如下:

```
(venv) tom@lambda:~/work/PyCharm/dm9051a_bench_python$ sudo $(which python) summary.py --db bench-20250312.db 
==================================
 Summary: Bench Program 1.0.4
==================================
Epoch: 2
Rx Loop: 16
Net Interface: enp1s0
Dest. MAC List: [] ==Polling 前就印出, 一定是空陣列==
Valid MAC begin: 00:00:00:00:00:00
Valid MAC end: FF:FF:FF:FF:FF:FF
Database Dir: /home/tom/work/PyCharm/dm9051a_bench_python
Cmd TimeOut: {'PollCommand': 3, 'RxCommand': 1, 'PhyCommand': 10}
Test Modes: {'100Full': False, '100Half': True, '10Full': False, '10Half': True}
----------------------------------------
EFUSE of Target 0, ID = 00:60:6e:11:11:01  ==Polling 回覆的 Targets==
    MAC Address 1 : FF:01:02:04:08:10
    MAC Address 2 : 20:40:80:01:02:04
    MAC Address 3 : 03:0C:36:7E:F0:8F
    Vendor ID     : 030C
    Product ID    : 367E
    TA Version    : 1.0
----------------------------------------
Target 0 RX count = 0040
Bench Start Time: 2025-03-12 02:16:34
Bench Finished Time: 2025-03-12 02:16:54
Bench Time Takes: 0:00:20
Error Statistics:
No Error
==================================
 End of Summary 
==================================
```
---
### Report 說明
---

- **Epoch**: 2 (回合)
- **Rx Loop**: 16 (每回合次數)
- **網路介面**: enp1s0
- **目標 MAC 清單**: 空（Polling 前列印，確定為空）
- **有效 MAC 範圍**: `00:00:00:00:00:00` ~ `FF:FF:FF:FF:FF:FF`
- **資料庫路徑**: `/home/tom/work/PyCharm/dm9051a_bench_python`
---
- **指令超時設定**:
    - PollCommand: 3s
    - RxCommand: 1s
    - PhyCommand: 10s
- **測試模式**:
    - 100Full: ❌
    - 100Half: ✅
    - 10Full: ❌
    - 10Half: ✅
---
#### **Polling 回覆的目標 (Target 0)**

- **ID**: `00:60:6e:11:11:01`
- **MAC Addresses**:
    1. `FF:01:02:04:08:10`
    2. `20:40:80:01:02:04`
    3. `03:0C:36:7E:F0:8F`
- **廠商 ID**: `030C`
- **產品 ID**: `367E`
- **TA 版本**: `1.0`
---
#### **測試結果**

- **Target 0 RX 計數**: `0040` <== epoch * n_loop \* Number of Test Modes :
	- 2 \* 16 \* 2 = 64 = 0x40
- **測試開始時間**: `2025-03-12 02:16:34`
- **測試結束時間**: `2025-03-12 02:16:54`
- **測試耗時**: `20 秒`
- **錯誤統計**: 無錯誤 ✅

---
## Appendix 

DataBase view : 觀看每一個指令的記錄

---

### 安裝 db viewer

- 儲存的 DB 是 SQLite, 任何 SQLite browser 都可以看

- Linux (PC or 樹莓派): `sudo apt install sqlitebrowser`

---
### 資料庫 viewer
[[公司文件/attachments/685533f648c8d1734396f83be9072104_MD5.jpeg|Open: Pasted image 20250324111556.png]]
![[公司文件/attachments/685533f648c8d1734396f83be9072104_MD5.jpeg]]

---

### 再產生 Report

如果測試後, 要再次看 Report , 使用指令: 
`python3 summary.py --db [Database file]

例如, 看 2024/12/27 那一天的 Report :
`python3 summary.py --db /tmp/bench-20241227.db`

---

> [!NOTE] 限制
> Database 以日期命名, 只能再產生當天最後一次測試的報告, 當天其他的測試, 需要用 DB Viewer 打開來看.


---

# Q & A

---