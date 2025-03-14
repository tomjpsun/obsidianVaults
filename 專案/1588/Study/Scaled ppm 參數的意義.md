### **`scaled_ppm` 參數的意義**

在 `igb_ptp_adjfine_82580(struct ptp_clock_info *ptp, long scaled_ppm)` 內，`scaled_ppm` 代表**相對於基準頻率的頻率偏移量**，它的單位是 **「ppm 的 2^16 倍」（parts per million, scaled by 2^16）**。

#### **換句話說**

- `scaled_ppm` 是 PTP 子系統提供的一個值，表示應該如何調整時鐘的頻率，使 PTP 時鐘保持同步。
- `scaled_ppm = ±(ppm × 2^16)`，所以如果：
    - `scaled_ppm = 65536`，表示**增加 1 ppm**（+1e-6）
    - `scaled_ppm = -131072`，表示**減少 2 ppm**（-2e-6）
    - `scaled_ppm = 0`，表示**不調整頻率**

---

### **如何使用 `scaled_ppm` 來調整時鐘**

1. **計算新的頻率增量**
    
    - 在 `igb_ptp_adjfine_82580()` 內，會根據 `scaled_ppm` 來計算新的時間增量值：
        
        ```c
        neg_adj = diff_by_scaled_ppm(IGB_82580_BASE_PERIOD, scaled_ppm, &rate);
        ```
        
        - `IGB_82580_BASE_PERIOD` 是 PTP 硬體時鐘的基準增量。
        - `diff_by_scaled_ppm()` 計算新的增量 `rate`，用來補償時間漂移。
2. **調整 `E1000_TIMINCA`（時間增量寄存器）**
    
    - `E1000_TIMINCA` 決定 PTP 硬體時鐘的增量值，影響時間流逝的速度：
        
        ```c
        inca = rate & INCVALUE_MASK;
        if (neg_adj)
            inca |= ISGN;
        E1000_WRITE_REG(hw, E1000_TIMINCA, inca);
        ```
        
    - 這會讓 PTP 時鐘的運行速度變快或變慢，補償時間漂移。

---

### **`scaled_ppm` 來自哪裡？**

1. **使用者空間程式（如 `ptp4l`）執行 `clock_adjtime()`**
    
    - `ptp4l` 會計算 PTP 硬體時鐘與參考時鐘（例如 GPS、NTP）的誤差，並使用 `ADJ_FREQUENCY` 選項來微調頻率：
        
        ```c
        struct timex tx;
        tx.modes = ADJ_FREQUENCY;
        tx.freq = new_freq;  // 這裡的 freq 是 parts per billion (ppb)
        clock_adjtime(fd, &tx);
        ```
        
    - 這會導致內核呼叫 `ptp_clock_info->adjfine()`，並傳入 `scaled_ppm` 值。
2. **Linux 內核轉換 `tx.freq`（ppb）為 `scaled_ppm`**
    
    - 內核中的 `posix-timers.c` 會進行轉換：
        
        ```c
        scaled_ppm = (ppb * 2^16) / 1000;
        ```
        
    - 如果 `tx.freq = 500` ppb，那麼：
        
        ```c
        scaled_ppm = (500 * 65536) / 1000 = 32768
        ```
        
    - 這表示**增加 0.5 ppm**。

---

### **總結**

- **`scaled_ppm` 代表應該如何調整 PTP 硬體時鐘的頻率，單位是 ppm 的 2^16 倍。**
- **當 PTP 軟體（如 `ptp4l`）發現時鐘有漂移時，它會計算 `scaled_ppm`，並請求 PTP 硬體調整頻率。**
- **驅動程式的 `igb_ptp_adjfine_82580()` 會根據 `scaled_ppm` 來修改 `E1000_TIMINCA`，影響 PTP 時鐘的運行速度，使其與參考時鐘同步。**