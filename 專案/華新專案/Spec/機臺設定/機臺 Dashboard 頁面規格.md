

1. **從 HMI configure  獲取機台列表API** (`POST /hmi_config)
_API parameters_

	tb1 = hmi_config,
	tb2 = machines,
	tb3 = virtual_devices 對應資料表名稱.

_API_
	 http://192.168.249.112:3002/catalog/hmi_configure/findReports
   - 功能：返回所有機台的列表和相關信息。
   - 輸出參數：
     - `machineId`: 機台的唯一識別碼。
     - `machineName`: 機台的名稱。
     - **獲取刷新間隔設定API** (`POST /settings/refresh-interval`)
	   - 功能：返回網頁自動刷新的時間間隔。
	   - 輸出參數：
	     - `interval`: 刷新間隔時間，單位為秒。



2. **獲取軸狀態信息API** (`POST /Production API`)
   - 功能：根據機台ID，返回該機台上所有軸的當前狀態信息。
   - 輸出參數：
     - `axleId`: 軸的唯一識別碼。
     - `currentState`: 軸的當前狀態。
     - `loadingTime`: 上軸時間。
     - `unloadingTime`: 出軸時間。
     - `productionTime`: 生產時間。
     - `command`: 製令。
     - `alert`: 警示公式，屬於 configure 設定，說明拿那些欄位運算，結果爲 true 就將 cell 標識成紅色




