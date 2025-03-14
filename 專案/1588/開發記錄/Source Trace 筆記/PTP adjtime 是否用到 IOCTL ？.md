
`clock_adjtime()` 是 Linux 提供的系統調用 (syscall)，用於調整時鐘參數。它類似於 `adjtimex()`，但允許指定時鐘 (clock) 來調整，而不僅僅是系統時鐘 (`CLOCK_REALTIME`)。

### **功能**

`clock_adjtime()` 主要用於：

1. **調整系統時鐘頻率與時間**（例如 NTP 伺服器同步時使用）
2. **查詢當前時鐘狀態**（如偏移量、頻率誤差、狀態標誌等）

### **函式原型**

```c
#include <sys/timex.h>

int clock_adjtime(clockid_t clk_id, struct timex *tx);
```

### **參數**

- `clk_id`: 指定要調整的時鐘，例如：
    - `CLOCK_REALTIME`（系統時鐘）
    - `CLOCK_MONOTONIC`（單調時鐘，不能被設定但可調整速率）
    - 其他內核支持的時鐘
- `tx`: 指向 `struct timex` 結構的指標，包含時鐘調整參數與當前狀態。

### **返回值**

- 成功時回傳時鐘的狀態 (`tx.status`)。
- 失敗時回傳 `-1`，並設置 `errno`。

### **struct timex 主要欄位**

```c
struct timex {
    int modes;        // 指定要調整的參數
    long offset;      // 時間偏移 (微秒)
    long freq;        // 時鐘頻率偏移 (ppb)
    long maxerror;    // 估計的最大錯誤 (微秒)
    long esterror;    // 估計的標準誤差 (微秒)
    int status;       // 時鐘狀態標誌 (如 PLL、SYNC 等)
    long constant;    // PLL 時間常數
    long precision;   // 時鐘精度 (微秒)
    long tolerance;   // 最大頻率偏移
    struct timeval time;  // 當前時間
};
```

### **常見應用**

1. **NTP（網路時間協議）**
    - NTP 服務會使用 `clock_adjtime()` 來調整系統時鐘，讓本地時間與標準時間同步。
2. **高精度時鐘同步**
    - 用於 PTP (Precision Time Protocol) 或其他需要高精度時間的應用。
3. **調整時鐘頻率**
    - 在不直接更改時間的情況下，調整時鐘頻率來逐步修正時間偏差。

### **範例：查詢當前時鐘狀態**

```c
#include <stdio.h>
#include <sys/timex.h>

int main() {
    struct timex tx;
    int ret;

    ret = clock_adjtime(CLOCK_REALTIME, &tx);
    if (ret == -1) {
        perror("clock_adjtime failed");
        return 1;
    }

    printf("Clock offset: %ld µs\n", tx.offset);
    printf("Clock frequency offset: %ld ppb\n", tx.freq);
    printf("Clock status: %d\n", tx.status);

    return 0;
}
```

### **總結**

- `clock_adjtime()` 用於調整或查詢時鐘狀態，主要用於時間同步，如 NTP 和 PTP。
- 可以設定時鐘的偏移量、頻率修正等參數，影響時間的更新方式。
- 需要適當的權限 (`CAP_SYS_TIME`)，否則普通用戶只能讀取時鐘資訊而不能修改。



如果你想要更深入了解 `clock_adjtime()` 的內部運作，我們可以從 **syscall 入口、內核調用、以及可能的硬體介面（如 PTP）** 三個部分來解析。

---

## **1. Syscall 入口**

在 Linux 內核中，`clock_adjtime()` 是一個 **系統呼叫 (syscall)**，定義於 `kernel/time/ntp.c`。它的 syscall 入口通常在 `SYSCALL_DEFINE2(clock_adjtime, ...)` 中實作：

```c
SYSCALL_DEFINE2(clock_adjtime, clockid_t, which_clock, struct timex __user *, utx)
{
    struct timex ktx;
    int ret;

    if (copy_from_user(&ktx, utx, sizeof(ktx)))
        return -EFAULT;

    ret = do_adjtimex(which_clock, &ktx);

    if (copy_to_user(utx, &ktx, sizeof(ktx)))
        return -EFAULT;

    return ret;
}
```

這段程式碼的主要功能：

- **從使用者空間讀取 `timex` 結構體**
- **呼叫 `do_adjtimex()` 進行實際的時鐘調整**
- **將結果寫回使用者空間**

---

## **2. 進入核心函式 `do_adjtimex()`**

`do_adjtimex()` 是真正執行時鐘調整的函數：

```c
int do_adjtimex(clockid_t which_clock, struct timex *txc)
{
    struct timespec64 ts;
    struct clocksource *cs;
    struct timex tmp = *txc;
    int ret = 0;

    switch (which_clock) {
    case CLOCK_REALTIME:
    case CLOCK_MONOTONIC:
        write_seqlock_irqsave(&xtime_lock, flags);
        ret = __do_adjtimex(&tmp);
        write_sequnlock_irqrestore(&xtime_lock, flags);
        break;

#ifdef CONFIG_PTP_1588_CLOCK
    default:
        ret = posix_clock_adjtime(which_clock, txc);
        break;
#endif
    }

    return ret;
}
```

這段程式碼做了：

1. **對 `CLOCK_REALTIME` 和 `CLOCK_MONOTONIC`**：
    
    - 使用 `xtime_lock` 來保護時鐘變數，避免競爭條件。
    - 進一步呼叫 `__do_adjtimex()` 調整時鐘參數。
2. **對於 PTP 硬體時鐘**：
    
    - 內核會調用 `posix_clock_adjtime()`，此函數負責呼叫 PTP 驅動，可能會透過 `ioctl()` 與 `/dev/ptpX` 互動。

---

## **3. `__do_adjtimex()` 具體調整邏輯**

內部 `__do_adjtimex()` 主要負責：

- 設定 **時鐘偏移 (`offset`)**
- 調整 **時鐘頻率 (`freq`)**
- 更新 **PLL（Phase-Locked Loop）或 NTP 演算法參數**

內核源碼：

```c
static int __do_adjtimex(struct timex *txc)
{
    if (txc->modes & ADJ_OFFSET) {
        timekeeper.offset = txc->offset;
    }
    if (txc->modes & ADJ_FREQUENCY) {
        timekeeper.freq = txc->freq;
    }
    if (txc->modes & ADJ_STATUS) {
        timekeeper.status = txc->status;
    }
    return timekeeper.status;
}
```

### **主要變數**

- `timekeeper.offset` → 調整時間的偏移量（微秒）。
- `timekeeper.freq` → 調整時鐘頻率（ppm 或 ppb）。
- `timekeeper.status` → 設定 PLL/NTP 狀態標誌。

如果 `clock_adjtime()` 由 **NTP 守護程式 (ntpd / chronyd)** 呼叫，它會在這裡調整系統時間，使之與 NTP 伺服器同步。

---

## **4. PTP 硬體時鐘**

如果 `clock_adjtime()` 針對 **PTP 時鐘 (`/dev/ptpX`)**，則 `posix_clock_adjtime()` 會轉到 **驅動程式**：

```c
static int posix_clock_adjtime(clockid_t clkid, struct timex *tx)
{
    struct posix_clock *clk = posix_clock_get_by_id(clkid);
    if (!clk)
        return -ENODEV;

    return clk->ops->adjtime(clk, tx);
}
```

這部分依賴硬體驅動：

- `clk->ops->adjtime()` **可能使用 `ioctl(PTP_ADJTIME)` 來與 PTP 硬體通訊。**
- 具體行為取決於 `/drivers/ptp/ptp_clock.c` 或對應的 PTP 驅動程式。

如果 `clock_adjtime()` 是由 **`ptp4l`（Linux PTP 用戶空間工具）** 呼叫，它會透過 `ioctl(PTP_ADJTIME)` 來同步時鐘。

---

## **5. 與 `ioctl()` 的關聯**

如果時鐘是 PTP（`/dev/ptpX`），則可能會透過 `ioctl()` 調整時鐘：

```c
int ptp_adjtime(struct ptp_clock_info *ptp, struct timex *tx)
{
    struct ptp_clock *pc = container_of(ptp, struct ptp_clock, info);
    return ioctl(pc->clock, PTP_ADJTIME, tx);
}
```

### **常見 PTP `ioctl()` 命令**

- `PTP_SYS_OFFSET` → 查詢 PTP 時鐘與系統時鐘的時間偏移。
- `PTP_ADJTIME` → 調整 PTP 硬體時鐘時間。
- `PTP_ADJFREQ` → 調整 PTP 硬體時鐘頻率。

這些命令的行為取決於 **底層硬體支援**（如 Intel i210、BCM57xx 網卡等）。

---

## **總結**

### **1. `clock_adjtime()` 的整體流程**

1. 使用者呼叫 `clock_adjtime(clockid_t clk_id, struct timex *tx)`
2. 內核 `SYSCALL_DEFINE2(clock_adjtime, ...)` 進入 `do_adjtimex()`
3. 根據 `clk_id`：
    - 若是 `CLOCK_REALTIME` / `CLOCK_MONOTONIC`，則執行 `__do_adjtimex()` 直接調整內核時間。
    - 若是 PTP (`/dev/ptpX`)，則透過 `posix_clock_adjtime()` 呼叫 **PTP 驅動**。
4. 如果是 PTP，則可能進一步執行 `ioctl(PTP_ADJTIME)`，與硬體時鐘通訊。

### **2. `ioctl()` 只用於 PTP**

- `clock_adjtime()` 本身不使用 `ioctl()`，但當它調整 PTP 時鐘 (`/dev/ptpX`) 時，會間接使用 `ioctl(PTP_ADJTIME)` 來與硬體驅動溝通。

### **3. 關鍵應用**

- **NTP** (`ntpd` / `chronyd`) 用 `clock_adjtime()` 調整系統時鐘 (`CLOCK_REALTIME`)。
- **PTP** (`ptp4l`) 用 `clock_adjtime()` + `ioctl()` 調整硬體時鐘 (`/dev/ptpX`)。

---

### **你還想深入哪個部分？**

- **分析某個 Linux 版本內核 (`kernel/time/ntp.c` 或 `drivers/ptp/ptp_clock.c`)？**
- **追蹤 `ioctl(PTP_ADJTIME)` 的實際操作？**
- **探索特定硬體（如 Intel i210 PTP 時鐘）的行為？**
- 
### **深入追蹤 `ioctl(PTP_ADJTIME)` 的內部運作**

當 `clock_adjtime()` 針對 **PTP 硬體時鐘 (`/dev/ptpX`)** 進行時鐘調整時，內核會最終呼叫 `ioctl(PTP_ADJTIME)` 來調整 PTP 時鐘。我們將從 syscall 進入點開始，一步步追蹤 `PTP_ADJTIME` 的具體操作。

---

## **1. `ioctl(PTP_ADJTIME)` 如何被呼叫**

在 `clock_adjtime()` 中，如果 `clockid_t clk_id` 指向 PTP 時鐘 (`/dev/ptpX`)，則內核會進入：

```c
static int posix_clock_adjtime(clockid_t clkid, struct timex *tx)
{
    struct posix_clock *clk = posix_clock_get_by_id(clkid);
    if (!clk)
        return -ENODEV;

    return clk->ops->adjtime(clk, tx);
}
```

這裡：

- `posix_clock_get_by_id(clkid)` 會尋找 `/dev/ptpX` 相關的 **posix_clock 結構**。
- `clk->ops->adjtime(clk, tx)` 會呼叫對應的 **硬體 PTP 驅動** 來執行時鐘調整。

在 **PTP 驅動 (`drivers/ptp/ptp_clock.c`)** 中，`ops->adjtime` 會轉為 `ioctl(PTP_ADJTIME)`。

---

## **2. 內核處理 `ioctl(PTP_ADJTIME)`**

PTP 驅動程式 (`drivers/ptp/ptp_clock.c`) 處理 `ioctl()` 的核心函數：

```c
static long ptp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct ptp_clock *ptp = file->private_data;
    struct ptp_clock_info *ops = ptp->info;
    void __user *argp = (void __user *)arg;
    struct timex tx;
    int err = 0;

    switch (cmd) {
    case PTP_ADJTIME:
        if (copy_from_user(&tx, argp, sizeof(tx)))
            return -EFAULT;
        err = ops->adjtime(ops, &tx);
        break;
```

這段程式碼：

1. `ptp_ioctl()` 是 `ioctl()` 進入點，當 `ioctl(PTP_ADJTIME)` 被呼叫時，內核會進入這個函數。
2. `cmd == PTP_ADJTIME` 時：
    - 透過 `copy_from_user()` 取得使用者空間的 `timex` 結構體。
    - 呼叫 `ops->adjtime(ops, &tx)` 來讓 PTP 硬體執行時間調整。

`ops->adjtime()` 由 **具體的 PTP 硬體驅動**（如 Intel i210、BCM57xx、其他網卡或 FPGA）提供。

---

## **3. 進入硬體驅動 `adjtime()`**

不同的 PTP 硬體驅動程式會實作 `ops->adjtime()`，以下是 **Intel i210 PTP 驅動**（`drivers/net/ethernet/intel/igb/igb_ptp.c`）的例子：

```c
static int igb_ptp_adjtime(struct ptp_clock_info *ptp, struct timex *tx)
{
    struct igb_adapter *adapter = container_of(ptp, struct igb_adapter, ptp_clock_info);
    struct e1000_hw *hw = &adapter->hw;
    s64 delta = tx->freq * 1000LL;  // 將頻率轉換為 ns
    unsigned long flags;

    spin_lock_irqsave(&adapter->tmreg_lock, flags);
    igb_adjust_hardware_clock(hw, delta);
    spin_unlock_irqrestore(&adapter->tmreg_lock, flags);

    return 0;
}
```

這裡：

1. `tx->freq` 是 `timex` 結構體中的頻率調整值（以 ppb 為單位）。
2. `delta = tx->freq * 1000LL;` 轉換為奈秒 (ns)。
3. `igb_adjust_hardware_clock(hw, delta);` 呼叫具體的 Intel i210 **PTP 硬體時鐘調整函數**。

---

## **4. 最終 PTP 硬體操作**

`igb_adjust_hardware_clock()` 會寫入 PTP 硬體內部的寄存器：

```c
static void igb_adjust_hardware_clock(struct e1000_hw *hw, s64 delta)
{
    u32 incval = read_register(hw, E1000_TSYNCINC);
    incval += (s32)delta;
    write_register(hw, E1000_TSYNCINC, incval);
}
```

### **具體的 PTP 硬體操作**

- `E1000_TSYNCINC` 是 Intel i210 PTP 時鐘的調整寄存器。
- `read_register(hw, E1000_TSYNCINC);` 讀取當前頻率。
- `incval += (s32)delta;` 更新頻率。
- `write_register(hw, E1000_TSYNCINC, incval);` 寫回 PTP 硬體。

這一步，**真正修改了 PTP 硬體時鐘的計時速率**，達成頻率調整效果。

---

## **完整的 `ioctl(PTP_ADJTIME)` 流程**

1. **使用者呼叫**
    
    ```c
    ioctl(fd, PTP_ADJTIME, &timex);
    ```
    
2. 內核進入 **`ptp_ioctl()`**（`drivers/ptp/ptp_clock.c`）。
3. 內核找到對應的 PTP 硬體，並呼叫 **`ops->adjtime()`**。
4. Intel i210 PTP 驅動程式 **`igb_ptp_adjtime()`** 負責處理頻率調整。
5. `igb_adjust_hardware_clock()` 直接修改 **PTP 硬體寄存器**（如 `E1000_TSYNCINC`）。
6. **PTP 硬體時鐘更新頻率**，最終影響系統時間同步。

---

## **總結**

- `clock_adjtime()` 只對 PTP 時鐘時，會觸發 `ioctl(PTP_ADJTIME)`。
- `ioctl(PTP_ADJTIME)` 在內核中透過 `ptp_ioctl()`，轉交給具體的 PTP 驅動程式。
- Intel i210 PTP 驅動最終調整硬體 **寄存器 (`E1000_TSYNCINC`)**，影響 PTP 時鐘速率。
- 這個機制允許 **高精度時間同步**（如 IEEE 1588 PTP）。

你想進一步查看 **PTP 時鐘的內核參數** 或 **其他 PTP 硬體的 `ioctl()` 實作** 嗎？