### **`get_raw_delay()` 詳細解析**

`get_raw_delay()` 是 `tsproc.c` 中負責**計算原始傳輸延遲（raw delay）** 的函式，主要用於時間同步系統（如 PTP, NTP）來計算本地與遠端設備之間的傳輸延遲。

---

### **函式定義**

```c
tmv_t get_raw_delay(struct tsproc *tsp)
```

- **參數：**
    
    - `tsp`：指向 `tsproc` 結構體的指標，代表時間戳處理器。
- **回傳值：**
    
    - 計算出的**原始延遲時間**，單位為 `tmv_t`（通常為奈秒）。

---

### **函式運作流程**

1️⃣ **確認時間戳記是否有效**

- 先從 `tsp`（時間戳處理器）結構體中讀取 `t2` 和 `t3` 的時間戳記：
- 這兩個值是本地裝置（Local）與遠端裝置（Remote）間的時間戳記。

2️⃣ **計算兩個時間差**

- `t23 = t2 - t3`（本地設備回應時間差）
- `t41 = t4 - t1`（遠端設備回應時間差）
- 這兩個時間差的意義：
    - `t23` 表示本地設備測得的時間延遲。
    - `t41` 表示遠端設備測得的時間延遲。

3️⃣ **應用時鐘頻率誤差補償**

- `t23` 會根據 `tsp->clock_rate_ratio`（時鐘速率比率）進行縮放：

```c
if (tsp->clock_rate_ratio != 1.0)
    t23 = dbl_tmv(tmv_dbl(t23) * tsp->clock_rate_ratio);
```

- `clock_rate_ratio` 表示遠端設備與本地設備的時鐘速率誤差，通常略微不同。
- 透過縮放 `t23`，可以補償這種誤差，確保計算出的延遲值更加準確。

4️⃣ **計算最終的原始延遲**

- `delay = (t23 + t41) / 2`
- 這個計算公式源於標準的**雙向測量時間同步算法（Two-Way Time Transfer Algorithm）**：$$ \text{raw delay} = \frac{(t2 - t3) \times \text{clock\_rate\_ratio} + (t4 - t1)}{2}$$
- 這個方法的核心概念是：
    - 兩個設備各自測量**發送與接收的時間**，然後將結果平均，減少網路異常延遲的影響。

5️⃣ **處理負延遲異常**

- 若計算出的 `delay` 小於 `0`，代表異常：

```c
if (tmv_sign(delay) < 0) {
    pr_debug("negative delay %10" PRId64, tmv_to_nanoseconds(delay));
}
```

- 這通常代表設備的時鐘不同步、或者網路傳輸異常。
- 可能需要進一步處理，例如：重新校準時鐘或排除網路問題。

6️⃣ **回傳計算出的原始延遲**

- 最後回傳 `delay` 值，提供給其他函式用於進一步的時間同步計算。

---

### **程式碼完整解析**

```c
tmv_t get_raw_delay(struct tsproc *tsp)
{
    tmv_t t23, t41, delay;

    /* 計算 t2 - t3，本地設備的時間間隔 */
    t23 = tmv_sub(tsp->t2, tsp->t3);

    /* 根據時鐘速率誤差進行補償 */
    if (tsp->clock_rate_ratio != 1.0)
        t23 = dbl_tmv(tmv_dbl(t23) * tsp->clock_rate_ratio);

    /* 計算 t4 - t1，遠端設備的時間間隔 */
    t41 = tmv_sub(tsp->t4, tsp->t1);

    /* 計算最終的原始延遲值 */
    delay = tmv_div(tmv_add(t23, t41), 2);

    /* 若發現延遲值為負數，輸出 debug 訊息 */
    if (tmv_sign(delay) < 0) {
        pr_debug("negative delay %10" PRId64, tmv_to_nanoseconds(delay));
        pr_debug("delay = (t2 - t3) * rr + (t4 - t1)");
        pr_debug("t2 - t3 = %+10" PRId64, tmv_to_nanoseconds(t23));
        pr_debug("t4 - t1 = %+10" PRId64, tmv_to_nanoseconds(t41));
        pr_debug("rr = %.9f", tsp->clock_rate_ratio);
    }

    /* 回傳計算出的原始延遲值 */
    return delay;
}
```

---

### **數學公式推導**

這段程式碼的計算原理可以用下列公式來解釋：

#### **雙向時間同步算法（Two-Way Time Transfer Algorithm）**

$$ \text{delay} = \frac{(T_2 - T_3) \times \text{clock\_rate\_ratio} + (T_4 - T_1)}{2} $$


- `T1`：設備 A 發送封包的時間。
- `T2`：設備 B 接收到封包的時間。
- `T3`：設備 B 回應封包的時間。
- `T4`：設備 A 接收到封包的時間。

#### **補償時鐘誤差**

時鐘速率誤差（Clock Drift）會影響同步準確度，因此補償公式：

$$T_2' = (T_2 - T_3) \times \text{clock\_rate\_ratio}$$

這樣能確保當遠端時鐘與本地時鐘頻率不同時，仍能獲得較準確的結果。

---

### **函式的用途**

1. **計算本地設備與遠端設備間的網路傳輸延遲**
    
    - `get_raw_delay()` 是時間同步的核心部分，確保不同設備間的時間對齊。
2. **提供給 `tsproc_update_offset()` 使用**
    
    - `tsproc_update_offset()` 需要延遲資訊來計算時間偏移量（offset）。
    - 原始延遲（raw delay）可以與過濾延遲（filtered delay）做比較，決定是否需要加權處理。
3. **修正時鐘速率誤差**
    
    - `clock_rate_ratio` 允許修正不同設備之間的時鐘頻率偏差，減少同步誤差。
4. **偵測異常網路延遲**
    
    - 若 `get_raw_delay()` 計算出的延遲為負數，代表網路或時鐘存在異常，應進行調整。

---

### **範例**

假設我們有一個 `tsproc` 物件 `tsp`，可以這樣使用：

```c
tmv_t delay = get_raw_delay(tsp);
printf("計算出的原始延遲: %ld ns\n", tmv_to_nanoseconds(delay));
```

如果發現異常延遲，可以檢查 `clock_rate_ratio` 或重新同步時鐘。

---

### **總結**

📌 **`get_raw_delay()` 在 PTP/NTP 同步機制中是關鍵的一環**，負責：

- **計算雙向傳輸延遲**。
- **補償時鐘誤差**，減少時間同步的偏差。
- **幫助同步演算法調整時鐘速率**。
- **檢測網路延遲異常，提升系統穩定性**。

透過這個函式，時間同步系統可以更精確地對齊不同設備的時鐘，確保數據一致性，對於高精度應用（如金融交易、5G、分佈式系統）至關重要！ 🎯