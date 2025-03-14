`tsproc_update_offset()` 函式的主要作用是**更新時間偏移量（offset）**，用來計算本地與遠端時間戳記之間的時間差，並可選擇是否加權處理。

---

### **函式定義**

```c
int tsproc_update_offset(struct tsproc *tsp, tmv_t *offset, double *weight)
```

- **參數說明：**
    
    - `tsp`：指向 `tsproc`（時間戳記處理）結構體的指標。
    - `offset`：指向用來儲存計算出的**時間偏移量**的變數。
    - `weight`：指向用來儲存計算出的**加權值**（如果需要加權處理）。
- **回傳值：**
    
    - 成功時回傳 `0`。
    - 若缺少有效的時間戳記，則回傳 `-1`，表示無法計算偏移量。

---

### **函式執行流程**

#### 1️⃣ **檢查時間戳記是否可用**

首先，函式會檢查 `tsp->t1`（遠端時間戳記）和 `tsp->t2`（本地時間戳記）是否已初始化：

- 若這些時間戳記仍為**零值**，則代表沒有足夠的數據可計算，直接回傳 `-1` 結束。

#### 2️⃣ **確定應使用的延遲值（delay）**

根據 `tsp->mode`（處理模式），決定要使用的延遲時間：

- **`TSPROC_FILTER`（過濾模式）**
    
    - 使用**過濾後的延遲**（filtered delay）。
    - 必須確保 `tsp->filtered_delay_valid` 為真，否則無法計算。
- **`TSPROC_RAW`（原始模式）或 `TSPROC_RAW_WEIGHT`（原始加權模式）**
    
    - 直接計算**原始延遲**，透過 `get_raw_delay()` 取得。
- **`TSPROC_FILTER_WEIGHT`（過濾加權模式）**
    
    - 需要**同時**取得過濾後的延遲與原始延遲，來進行後續加權計算。

#### 3️⃣ **計算時間偏移量（offset）**

一旦決定應使用的 `delay`，函式會計算**時間偏移量**：

offset=(t2−t1)−delay\text{offset} = (t2 - t1) - \text{delay}

- `t2`：本地時間戳記。
- `t1`：遠端時間戳記。
- `delay`：延遲時間，根據模式選擇過濾後或原始延遲。

#### 4️⃣ **計算加權值（weight，可選）**

如果 `weight` 指標**不為空**，則計算加權值：

weight=filtered delayraw delay\text{weight} = \frac{\text{filtered delay}}{\text{raw delay}}

- 確保 `weight` 在 `[0,1]` 之間：
    - 若 `filtered delay > raw delay`，則強制設為 `1.0`。
    - 若 `filtered delay` 或 `raw delay` 為負數，則直接使用 `1.0`。

#### 5️⃣ **回傳成功**

如果一切計算成功，則回傳 `0`，表示偏移量更新成功。

---

### **函式特點**

✅ **支援多種處理模式**：

- **原始模式（RAW）**：不經過過濾，直接計算時間差。
- **過濾模式（FILTER）**：透過濾波器來平滑時間延遲數據。
- **加權模式（WEIGHT）**：計算出權重，讓時間同步演算法能更可靠。

✅ **確保數據有效性**：

- 若無法獲得足夠的時間戳記，會安全地回傳 `-1`，避免產生錯誤結果。

✅ **考慮時間延遲補償**：

- 透過 `get_raw_delay()` 來修正不對稱的延遲影響，確保計算出的時間偏移量更準確。

✅ **提供權重機制**：

- 若 `filtered delay` 的數據品質較好，則可以讓權重較高，使其對最終同步結果影響更大。

---

### **使用範例**

假設有一個 `tsproc` 物件 `tsp`，我們可以這樣呼叫 `tsproc_update_offset()` 來計算時間偏移：

```c
tmv_t offset;
double weight;
if (tsproc_update_offset(tsp, &offset, &weight) == 0) {
    printf("計算成功，時間偏移量：%ld ns，權重：%f\n", 
           tmv_to_nanoseconds(offset), weight);
} else {
    printf("計算失敗，時間戳記不足。\n");
}
```

這樣可以確保：

- **只有當數據可用時才會計算時間偏移量**。
- **提供權重值**，讓時間同步演算法能更好地調整時鐘。

---

### **總結**

📌 `tsproc_update_offset()` 在時間同步過程中扮演關鍵角色，能夠：

- 計算兩個時鐘間的**時間偏移量**。
- 適應不同的**同步模式**（原始、過濾、加權）。
- **確保時間延遲補償**，減少不對稱影響。
- **加權計算**，提供更準確的時間同步資訊。

這使得它在**精準時間同步（PTP, NTP, IEEE 1588）**應用中非常重要！ 🎯