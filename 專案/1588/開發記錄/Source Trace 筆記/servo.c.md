`servo.c` 是 LinuxPTP 的 **伺服控制（Servo Control）** 模組，負責調整本地時鐘，使其與 PTP 主時鐘保持同步。它提供了一個抽象層，允許使用不同類型的伺服控制方法，如 **PI 控制、線性回歸 (LinReg)、NTP 共享記憶體 (NTP SHM)** 等。

---

## **1. `servo.c` 的主要功能**

### **(1) 伺服控制類型**

在 `servo_create()` 函數中，`servo.c` 根據 **伺服控制類型 (`enum servo_type`)** 創建對應的伺服器：

```c
struct servo *servo_create(struct config *cfg, enum servo_type type,
			   double fadj, int max_ppb, int sw_ts)
{
    struct servo *servo;

    switch (type) {
    case CLOCK_SERVO_PI:
        servo = pi_servo_create(cfg, fadj, sw_ts);  // 比例-積分控制 (PI Control)
        break;
    case CLOCK_SERVO_LINREG:
        servo = linreg_servo_create(fadj);  // 線性回歸 (Linear Regression)
        break;
    case CLOCK_SERVO_NTPSHM:
        servo = ntpshm_servo_create(cfg);  // NTP 共享記憶體 (NTP SHM)
        break;
    case CLOCK_SERVO_NULLF:
        servo = nullf_servo_create();  // 空伺服器 (不做任何調整)
        break;
    case CLOCK_SERVO_REFCLOCK_SOCK:
        servo = refclock_sock_servo_create(cfg);  // 參考時鐘 Socket
        break;
    default:
        return NULL;
    }
    return servo;
}
```

🔹 **常見伺服控制類型：**

- `CLOCK_SERVO_PI`：使用 PI 控制（適用於大部分情況）。
- `CLOCK_SERVO_LINREG`：使用線性回歸估算時鐘漂移（適用於高精度需求）。
- `CLOCK_SERVO_NTPSHM`：使用 NTP 共享記憶體與 `ntpd` 互動（適用於混合時鐘同步）。
- `CLOCK_SERVO_NULLF`：不進行時鐘同步（測試用）。
- `CLOCK_SERVO_REFCLOCK_SOCK`：使用 Socket 接收外部時鐘源資訊。

---

### **(2) 伺服控制的核心函數**

這些函數負責時鐘同步的主要邏輯：

|**函數名稱**|**功能**|
|---|---|
|`servo_sample()`|接收新的時間偏移樣本，更新時鐘調整值|
|`servo_sync_interval()`|設定同步間隔|
|`servo_reset()`|重置伺服狀態|
|`servo_rate_ratio()`|計算時鐘速率比率|
|`servo_leap()`|處理閏秒|

---

## **2. `servo_sample()` - 主要時鐘同步函數**

這是 `servo.c` 中最關鍵的函數，負責接收 PTP 訊息的時間偏移 (`offset`)，並計算時鐘應如何調整：

```c
double servo_sample(struct servo *servo,
                    int64_t offset,
                    uint64_t local_ts,
                    double weight,
                    enum servo_state *state)
{
    double r;

    /* 呼叫特定伺服類型的 sample 函數 */
    r = servo->sample(servo, offset, local_ts, weight, state);

    switch (*state) {
    case SERVO_UNLOCKED:
        servo->curr_offset_values = servo->num_offset_values;
        break;
    case SERVO_JUMP:
        servo->curr_offset_values = servo->num_offset_values;
        servo->first_update = 0;
        break;
    case SERVO_LOCKED:
        if (check_offset_threshold(servo, offset)) {
            *state = SERVO_LOCKED_STABLE;
        }
        servo->first_update = 0;
        break;
    case SERVO_LOCKED_STABLE:
        /* 已鎖定狀態 */
        break;
    }

    return r;
}
```

🔹 **關鍵邏輯**

- `servo->sample()`：根據 `servo_type` 計算新的調整值（例如 PI 控制）。
- 根據時鐘同步狀態 (`servo_state`) 進行不同的動作：
    - `SERVO_UNLOCKED`：未鎖定，等待更多數據。
    - `SERVO_JUMP`：大幅跳變時鐘。
    - `SERVO_LOCKED`：進入鎖定狀態，持續調整時鐘。
    - `SERVO_LOCKED_STABLE`：完全鎖定，進行小幅調整。

---

## **3. `servo_sync_interval()` - 設定同步間隔**

這個函數設定 **時鐘同步的頻率**：

```c
void servo_sync_interval(struct servo *servo, double interval)
{
    servo->sync_interval(servo, interval);
}
```

🔹 **應用**

- 在 **高精度同步** 中，降低同步間隔可提高時鐘穩定度。
- 在 **低頻率環境**（如 IoT 裝置）中，可增加同步間隔以減少資源消耗。

---

## **4. `servo_reset()` - 重置伺服狀態**

當時鐘跳變或狀態不穩定時，這個函數會重新初始化伺服：

```c
void servo_reset(struct servo *servo)
{
    servo->reset(servo);
}
```

🔹 **應用**

- 當 PTP 重新選擇新的主時鐘時，重置伺服器以避免同步問題。

---

## **5. `servo_rate_ratio()` - 計算時鐘速率比率**

這個函數計算本地時鐘與主時鐘的 **速率比率 (Rate Ratio)**：

```c
double servo_rate_ratio(struct servo *servo)
{
    if (servo->rate_ratio)
        return servo->rate_ratio(servo);
    return 1.0;
}
```

🔹 **應用**

- 在 **自由運行模式 (Free-Running Mode)** 下，速率比率可用來估算時鐘漂移。

---

## **6. `servo_leap()` - 處理閏秒**

當 PTP 伺服器發送 **閏秒 (Leap Second) 訊息** 時，這個函數會調整時鐘：

```c
void servo_leap(struct servo *servo, int leap)
{
    if (servo->leap)
        servo->leap(servo, leap);
}
```

🔹 **應用**

- 當全球 UTC 時間變更時（如 2016 年 12 月 31 日 UTC 23:59:60）。
- 避免因閏秒導致的時間跳變問題。

---

## **7. `check_offset_threshold()` - 確保時鐘同步穩定**

這個函數檢查時鐘偏移是否超過允許範圍：

```c
static int check_offset_threshold(struct servo *s, int64_t offset)
{
    long long int abs_offset = llabs(offset);

    if (s->offset_threshold) {
        if (abs_offset < s->offset_threshold) {
            if (s->curr_offset_values)
                s->curr_offset_values--;
        } else {
            s->curr_offset_values = s->num_offset_values;
        }
        return s->curr_offset_values ? 0 : 1;
    }
    return 0;
}
```

🔹 **應用**

- 避免因 **網路延遲抖動 (Jitter)** 造成不必要的時鐘調整。
- 只有當偏移量 (`offset`) **連續超過閥值** 才進行調整，防止短暫波動影響同步。

---

## **8. `servo_offset_threshold()` - 取得時鐘同步閾值**

```c
int servo_offset_threshold(struct servo *servo)
{
    return servo->offset_threshold;
}
```

🔹 **應用**

- 允許用戶設定最小時間偏移 (`offset_threshold`)，避免頻繁微調。

---

## **9. 總結**

🔹 **`servo.c` 負責時鐘同步的核心**

1. **選擇伺服控制方法 (`servo_create()`)**：
    
    - **PI 控制** (`pi_servo_create()`)：適用於大多數場景。
    - **線性回歸 (`linreg_servo_create()`)**：適用於高精度應用。
    - **NTP 共享記憶體 (`ntpshm_servo_create()`)**：與 `ntpd` 配合使用。
2. **同步流程 (`servo_sample()`)**：
    
    - 接收新的時間偏移 (`offset`)。
    - 根據 PID 控制或線性回歸計算頻率調整值。
    - 更新時鐘頻率 (`clockadj_set_freq()`) 或進行時間跳變 (`clockadj_step()`)。
3. **穩定機制 (`check_offset_threshold()`)**：
    
    - 防止小幅波動導致不必要的時鐘調整。

