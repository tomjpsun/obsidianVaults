
1. **獲取機台列表API** (`GET /machines`)
   - 功能：返回所有機台的列表和相關信息。
   - 輸出參數：
     - `machineId`: 機台的唯一識別碼。
     - `machineName`: 機台的名稱。
     - `machineType`: 機台的類型（捲取軸或給線軸）。

2. **獲取軸狀態信息API** (`GET /axle-status/{machineId}`)
   - 功能：根據機台ID，返回該機台上所有軸的當前狀態信息。
   - 輸出參數：
     - `axleId`: 軸的唯一識別碼。
     - `currentState`: 軸的當前狀態。
     - `loadingTime`: 上軸時間。
     - `unloadingTime`: 出軸時間。
     - `productionTime`: 生產時間。
     - `command`: 製作命令。
     - `alert`: 警示信息。

3. **更新軸狀態API** (`PUT /axle-status/{axleId}`)
   - 功能：更新指定軸的狀態信息。
   - 輸入參數：
     - `currentState`: 軸的當前狀態。
     - `loadingTime`: 上軸時間。
     - `unloadingTime`: 出軸時間。
     - `productionTime`: 生產時間。
     - `command`: 製作命令。
     - `alert`: 警示信息。

4.  _optional_  **獲取刷新間隔設定API** (`GET /settings/refresh-interval`)
   - 功能：返回網頁自動刷新的時間間隔。
   - 輸出參數：
     - `interval`: 刷新間隔時間，單位為秒。


