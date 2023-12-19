# 資料庫設計
## 資料流程規劃
![[華新麗華-資料庫規劃-09.drawio.png]]

原始檔案: ./img/華新麗華-資料庫規劃-09.drawio


## 資料表ER圖
![[walsin_database_reel.png]]


網址: https://dbdiagram.io/d/walsin_database_reel-64d3336502bd1c4a5e7a1bbf


## 資料表說明
### app_config 資料表
![[Pasted image 20231130153943.png]]

HMI系統初始化設定表

原始檔案: ./csv/app_config.csv

### hmi_config 資料表
![[Pasted image 20231130154621.png]]

HMI平板電腦編號表
原始檔案: ./csv/hmi_config.csv

### machines 資料表
![[Pasted image 20231130155622.png]]

machine機台編號表
原始檔案: ./csv/machines.csv

### productions 資料表
![[Pasted image 20231130160024.png]]

productions 即時生產紀錄表
原始檔案: ./csv/productions.csv

### productions_log 資料表
![[Pasted image 20231130161047.png]]

productions_log 生產歷史紀錄表
原始檔案: ./csv/productions_log.csv


### reels 資料表
![[Pasted image 20231130161546.png]]

reels 周轉軸編號表
原始檔案: ./csv/reels.csv


### tag_identifier 資料表
![[Pasted image 20231130161958.png]]

tag_identifier 標籤編號表
原始檔案: ./csv/tag_identifier.csv

### virtual_devices 資料表
![[Pasted image 20231130162220.png]]

virtual_devices 天線編號表
原始檔案: ./csv/virtual_devices.csv


### workorders 資料表
![[Pasted image 20231130162518.png]]

workorders 生產製令紀錄表
原始檔案: ./csv/workorders.csv


### data_analyzer_log  資料表
![[Pasted image 20231130162843.png]]

data_analyzer_log CORE分析逐筆資料紀錄表
原始檔案: ./csv/data_analyzer_log.csv



### reader_device_state 資料表
![[Pasted image 20231130163639.png]]

reader_device_state reader即時更新狀態紀錄表
原始檔案: ./csv/reader_device_state.csv


### tag_reporting 資料表
![[Pasted image 20231130164122.png]]

tag_reporting tag 逐筆紀錄表
原始檔案: ./csv/tag_reporting.csv


# API設計
## UI關聯資料庫使用API
1. HMI關聯查詢
```json
//http://192.168.249.112:3002/catalog/hmi_configure/findReports
// method: post
// body:
{
      "data":
          {
              "tb1.name": "HMI033"
          }
}

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.hmi_config findSearch successfully.",
    "data": [
        {
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "hmi_comment": "HMI033",
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "device_id": 145,
            "device_name": "ST03G01RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.168.249.51",
            "device_antennas": 1,
            "device_type": 1100,
            "device_status": null,
            "device_comment": "給線滿軸準備區(1100)",
            "device_parent_id": 155
        },
        {
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "hmi_comment": "HMI033",
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "device_id": 146,
            "device_name": "ST03G02RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.167.51.51",
            "device_antennas": 2,
            "device_type": 1100,
            "device_status": null,
            "device_comment": "給線滿軸準備區(1100)",
            "device_parent_id": 156
        },
		
		~~~~ 列表所以裝置
	]
}

```

## CORE關聯資料庫使用API
1. analyzer_result_join 關聯查詢
```json
// http://192.168.249.112:3002/catalog/data_analyzer/analyzer_result_join
// method: post
// body:
{
    "data": [
        {
            "device_ip": "192.168.249.51",
            "antenna": 1,
            "epc": "35412204000000442240B051"

        }
	]
}

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.data_analyzer_log find successfully.",
    "data": [
        {
            "id": 2677,
            "reel_number": "D2240X051",
            "name": "442240B051",
            "uid": null,
            "tid": "E280117020001057FAB90B5F",
            "epc": "35412204000000442240B051",
            "axis_surface": "B",
            "status": null,
            "reserved": null,
            "device_id": 145,
            "device_name": "ST03G01RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.168.249.51",
            "device_antennas": 1,
            "device_position": 1100,
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "reel_tax_id": "",
            "reel_reserved": "",
            "reel_axis_code": "D(周轉軸)",
            "reel_axis_size": "2240",
            "reel_axis_surface": "A/B",
            "reel_serial_number": "051"
        },
        {
            "id": 2678,
            "reel_number": "D2240X051",
            "name": "442240B051",
            "uid": null,
            "tid": "E2801170200014F7FAB20B5F",
            "epc": "35412204000000442240B051",
            "axis_surface": "B",
            "status": null,
            "reserved": null,
            "device_id": 145,
            "device_name": "ST03G01RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.168.249.51",
            "device_antennas": 1,
            "device_position": 1100,
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "reel_tax_id": "",
            "reel_reserved": "",
            "reel_axis_code": "D(周轉軸)",
            "reel_axis_size": "2240",
            "reel_axis_surface": "A/B",
            "reel_serial_number": "051"
        },
        {
            "id": 2679,
            "reel_number": "D2240X051",
            "name": "442240B051",
            "uid": null,
            "tid": "E2801170200010F7FAB90B5F",
            "epc": "35412204000000442240B051",
            "axis_surface": "B",
            "status": null,
            "reserved": null,
            "device_id": 145,
            "device_name": "ST03G01RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.168.249.51",
            "device_antennas": 1,
            "device_position": 1100,
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "reel_tax_id": "",
            "reel_reserved": "",
            "reel_axis_code": "D(周轉軸)",
            "reel_axis_size": "2240",
            "reel_axis_surface": "A/B",
            "reel_serial_number": "051"
        },
        {
            "id": 2680,
            "reel_number": "D2240X051",
            "name": "442240B051",
            "uid": null,
            "tid": "E280117020001097FAB90B5F",
            "epc": "35412204000000442240B051",
            "axis_surface": "B",
            "status": null,
            "reserved": null,
            "device_id": 145,
            "device_name": "ST03G01RF01",
            "device_full_name": "給線滿軸準備區(1100)",
            "device_ip": "192.168.249.51",
            "device_antennas": 1,
            "device_position": 1100,
            "machine_id": 33,
            "machine_number": "ST03",
            "machine_name": "1600雙扭機",
            "machine_full_name": "1600雙扭機",
            "machine_floor": 1,
            "machine_type": 0,
            "machine_status": 0,
            "hmi_id": 33,
            "hmi_name": "HMI033",
            "hmi_full_name": "HMI033",
            "hmi_ip": "192.168.249.82",
            "hmi_type": null,
            "hmi_status": null,
            "hmi_idts_core_status": null,
            "hmi_idts_data_status": null,
            "hmi_data": null,
            "reel_tax_id": "",
            "reel_reserved": "",
            "reel_axis_code": "D(周轉軸)",
            "reel_axis_size": "2240",
            "reel_axis_surface": "A/B",
            "reel_serial_number": "051"
        }
    ]
}


```

2. analyzer_result_tag 關聯查詢
	以 reel_number查詢 epc, 以epc查詢 reel_number 列表.
	
```json
// http://192.168.0.143:3002/catalog/data_analyzer/analyzer_result_tag
// method: post
// body:
{
    "data": 
        {
            // "tb1.epc": "35412204000000440800B002"
            "tb1.reel_number": "D0800X002"
        }
	
}

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.tag_identifier find successfully.",
    "data": [
        {
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": null,
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A",
            "serial_number": "002",
            "id": 3,
            "name": "440800A002",
            "uid": null,
            "tid": "E280117020001516FAD00B5F",
            "epc": "35412204000000440800A002",
            "status": null
        },
        {
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": null,
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A",
            "serial_number": "002",
            "id": 4,
            "name": "440800A002",
            "uid": null,
            "tid": "E2801170200013B6FACD0B5F",
            "epc": "35412204000000440800A002",
            "status": null
        },
        {
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": null,
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "B",
            "serial_number": "002",
            "id": 449,
            "name": "440800B002",
            "uid": null,
            "tid": "E280117020001593FAB40B5F",
            "epc": "35412204000000440800B002",
            "status": null
        },
        {
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": null,
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "B",
            "serial_number": "002",
            "id": 450,
            "name": "440800B002",
            "uid": null,
            "tid": "E280117020001533FAB40B5F",
            "epc": "35412204000000440800B002",
            "status": null
        },
        {
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": "DEMO001",
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A",
            "serial_number": "002",
            "id": 2690,
            "name": "440800A002",
            "uid": null,
            "tid": "E280110520007B8EA6970A68",
            "epc": "AAAAAAAAAAAAAAB630000086",
            "status": null
        }
    ]
}




```


## 資料表 API
### app_config API
#### create
```json
// http://192.168.249.114:3002/catalog/api_configure/creates
// method: post
// body:
{
"eventname": "test",    
"data": [
        {
            "uid": "c5f1d942-70ff-42cd-8e09-168638073909",
            "data": "{\r\n \"backend_port\": 80,\r\n \"backend_url\": \"192.168.249.50\",\r\n \"hmi_id\": \"1\",\r\n \"log_switch\": \"CONSOLE\"\r\n}"
        }
    ]
}

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.app_config update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 1,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

#### read

```json
// http://192.168.249.114:3002/catalog/app_configure/findsearch
// method: get
// body: none

// resp: json file.
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.app_config findSearch successfully.",
    "data": [
        {
            "id": 99320,
            "uid": "33",
            "data": "{\"backend_url\":\"http://127.0.0.1\",\"backend_port\":\"3002\",\"hmi_id\":\"33\",\"log_switch\":\"CONSOLE\",\"dashboard_height\":\"800px\"}"
        },
        {
            "id": 99093,
            "uid": "1",
            "data": "{\"backend_url\":\"http://192.168.249.112\",\"backend_port\":\"3002\",\"hmi_id\":\"1\",\"log_switch\":\"CONSOLE\"}"
        },
        {
            "id": 88,
            "uid": "c5f1d942-70ff-42cd-8e09-168638073909",
            "data": "{\r\n \"backend_port\": 80,\r\n \"backend_url\": \"192.168.249.50\",\r\n \"hmi_id\": \"1\",\r\n \"log_switch\": \"CONSOLE\"\r\n}"
        }
    ]
}


```

#### update
```json
// http://192.168.249.114:3002/catalog/api_configure/creates
// method: post
// body:
{
"eventname": "test",    
"data": [
        {
            "uid": "c5f1d942-70ff-42cd-8e09-168638073909",
            "data": "{\r\n \"backend_port\": 80,\r\n \"backend_url\": \"192.168.249.50\",\r\n \"hmi_id\": \"1\",\r\n \"log_switch\": \"CONSOLE\"\r\n}"
        }
    ]
}

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.app_config update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 1,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}
```

#### delete
```json
// http://192.168.249.112:3002/catalog/app_configure/deleteIDs
// method: delete
// body:
{
  "ids": ["4","10"]
}

response: 
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('4', '10')"
}

```

### hmi_config API
#### create
```js
// http://192.168.249.123:3002/catalog/hmi_configure/creates
// method: post
// body:
{
//   "eventname":"hmi_config"",  // table name (optional)
    "data": [
        {
            "id": 1,
            "name": "HMI001",
            "full_name": "HMI001",
            "address_ip": "192.168.249.50",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI001"
        },
        {
            "id": 2,
            "name": "HMI002",
            "full_name": "HMI002",
            "address_ip": "192.168.249.51",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI002"
        }    
    ]
}


response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```


#### read
```json
// http://192.168.249.123:3002/catalog/hmi_configure/findsearch
// method: get
// body: none

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.hmi_config findSearch successfully.",
    "data": [
        {
            "id": 1,
            "name": "HMI001",
            "full_name": "HMI001",
            "address_ip": "192.168.249.50",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI001"
        },
        {
            "id": 2,
            "name": "HMI002",
            "full_name": "HMI002",
            "address_ip": "192.168.249.51",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI002"
        }
    ]
}
```
#### update
```json

// http://192.168.249.123:3002/catalog/hmi_configure/creates
// method: post
// body:
{
//   "eventname":"hmi_config"",  // table name (optional)
    "data": [
        {
            "id": 1,
            "name": "HMI001",
            "full_name": "HMI001",
            "address_ip": "192.168.249.50",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI001"
        },
        {
            "id": 2,
            "name": "HMI002",
            "full_name": "HMI002",
            "address_ip": "192.168.249.51",
            "type": null,
            "status": null,
            "idts_core_status": null,
            "idts_data_status": null,
            "data": null,
            "reserved": null,
            "comment": "HMI002"
        }    
    ]
}


response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```
#### delete
```json

/ http://192.168.249.104:3002/catalog/hmi_configure/deleteIDs
// method: delete
// body:
{
  // "eventname":"users",   // eventname (optional)
  "ids": ["5","15"]      // ids array (required)
}

// response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5', '15
    ')"
}

```

### machines API
#### create
```json

// http://192.168.249.123:3002/catalog/machines/creates
// method: post
// body:
{
    // "eventname":"machines",    // eventname (optional)
	"data": [                  // data array (required)
		{
			// "id": 1,   // 新增id為資料庫自動產生，update 時需上傳
            "machine_number": "BS01",
            "name": "1+4集合機",
            "full_name": "1+4集合機",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 1,
            "comment": "1+4集合機"
		},
		{
			// "id": 2,   // 新增id為資料庫自動產生，update 時需上傳
            "machine_number": "CS01",
            "name": "61B同心絞",
            "full_name": "61B同心絞",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 2,
            "comment": "61B同心絞"		
        }	
	]
};


response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}
```

#### read
```json
// 以參數查詢
// http://192.168.123.183:3002/catalog/machines/findsearch?limit=2
// method: get
// body: none

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.machines findSearch successfully.",
    "data": [
        {
            "id": 1,
            "machine_number": "BS01",
            "name": "1+4集合機",
            "full_name": "1+4集合機",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 1,
            "comment": "1+4集合機"
        },
        {
            "id": 2,
            "machine_number": "CS01",
            "name": "61B同心絞",
            "full_name": "61B同心絞",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 2,
            "comment": "61B同心絞"
        },
        {
            "id": 3,
            "machine_number": "CS03",
            "name": "1600弓絞A",
            "full_name": "1600弓絞A",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 3,
            "comment": "1600弓絞A"
        }
    ]
}

```
#### update
```json

// http://192.168.249.123:3002/catalog/machines/creates
// method: post
// body:
{
    // "eventname":"machines",    // eventname (optional)
	"data": [                  // data array (required)
		{
			// "id": 1,   // 新增id為資料庫自動產生，update 時需上傳
            "machine_number": "BS01",
            "name": "1+4集合機",
            "full_name": "1+4集合機",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 1,
            "comment": "1+4集合機"
		},
		{
			// "id": 2,   // 新增id為資料庫自動產生，update 時需上傳
            "machine_number": "CS01",
            "name": "61B同心絞",
            "full_name": "61B同心絞",
            "floor": 1,
            "type": 0,
            "status": 0,
            "hmi_config_id": 2,
            "comment": "61B同心絞"		
        }	
	]
};


response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```
#### detete
```json

// http://192.168.249.123:3002/catalog/machines/deleteIDs
// method: delete
// body:
{
  "ids": ["5","15"]
}


response: 
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}
```
### productions API
#### create
```json
// http://192.168.123.183:3002/catalog/productions/creates
// http://192.168.249.112:3002/catalog/productions/findsearch
// method: post
// body:
{
//   "eventname":"productions",  // table name (optional)
    "data":[
        {
            "order_number": "",
            "reel_number": "D0800X005",
            "virtual_devices_id": 145,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        },
        {
            "order_number": "",
            "reel_number": "D0800X004",
            "virtual_devices_id": 153,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        }
    ]
}

response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```
#### read
```json
http://192.168.123.183:3002/catalog/productions/findsearch
// method: get
// body: none

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.productions findSearch successfully.",
    "data": [
        {
            "id": 4987,
            "order_number": "",
            "reel_number": "D0800X005",
            "virtual_devices_id": 145,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        },
        {
            "id": 4985,
            "order_number": "",
            "reel_number": "D0800X004",
            "virtual_devices_id": 153,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        },
        {
            "id": 4984,
            "order_number": "",
            "reel_number": "D0800X001",
            "virtual_devices_id": 160,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        }
    ]
}

```

#### update
```json
// http://192.168.123.183:3002/catalog/productions/creates
// http://192.168.249.112:3002/catalog/productions/creates
// method: post
// body:
{
//   "eventname":"productions",  // table name (optional)
    "data":[
        {
            "order_number": "",
            "reel_number": "D0800X005",
            "virtual_devices_id": 145,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        },
        {
            "order_number": "",
            "reel_number": "D0800X004",
            "virtual_devices_id": 153,
            "up_time": "2023-11-29 18:05:37.000",
            "down_time": null,
            "production_time": "2023-11-29 18:17:57.000",
            "average_line_speed": 0,
            "real_time_meter": 0,
            "created": "2023-11-29 18:05:37.000",
            "updated": "2023-11-29 18:17:57.000",
            "status": 1,
            "comment": "The reel is in operation on the machine."
        }
    ]
}

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.productions update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}

```
#### detete
```json
// http://192.168.249.112:3002/catalog/productions/deleteIDs
// method: delete
// body:
{
  "ids": ["4940","4895"]
}

// body:
{
 "ids": ["4","10"]
}



response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('4940', '4895')"
}

```



### productions_log API
#### create
```json

// http://192.168.123.183:3002/catalog/productions_log/creates
// method: post
// body:
{
//   "eventname":"productions",  // table name (optional)
    "data":[
        {
            "order_number": "123456789",
            "reel_number": "D0800X004",
            "virtual_devices_id": 9,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        },
        {
            "order_number": "123456789",
            "reel_number": "D0800X003",
            "virtual_devices_id": 155,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        }

    ]
}

esponse: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```
#### read
```json
// http://192.168.123.183:3002/catalog/productions_log/findsearch
// http://192.168.123.183:3002/catalog/productions_log/findsearch?limit=1000
// method: get
// body: none

// response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.productions findSearch successfully.",
    "data": [
        {
            "id": 22,
            "order_number": "123456789",
            "reel_number": "D0800X004",
            "virtual_devices_id": 9,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        },
        {
            "id": 21,
            "order_number": "123456789",
            "reel_number": "D0800X003",
            "virtual_devices_id": 155,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        }
    ]
}


```
#### update
```json
// http://192.168.123.183:3002/catalog/productions_log/creates
// method: post
// body:
{
//   "eventname":"productions",  // table name (optional)
    "data":[
        {
            "id": 122,
            "order_number": "123456789",
            "reel_number": "D0800X004",
            "virtual_devices_id": 9,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        },
        {
            "id": 123,
            "order_number": "123456789",
            "reel_number": "D0800X003",
            "virtual_devices_id": 155,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 2,
            "comment": "production0222"
        }

    ]
}

response: 

{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 122
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 4,
            "insertId": 122,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}
```




#### detete
```json

// http://192.168.249.104:3002/catalog/productions_log/deleteIDs
// method: delete
// body:
{
  //"eventname":"productions",   // eventname (optional)
  "ids": ["5","15"]      // ids array (required)
}

// response: 
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

### reels API
#### create
```json
// http://192.168.249.112:3002/catalog/reels/creates
// method: post
// body:
{
    // "eventname":"reels",    // eventname (optional)
	"data": [                  // data array (required)
		{
            "reel_number": "D0800X001",
            "tax_id": "",
            "reserved": "",
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A/B",
            "serial_number": "001"
		},
		{
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": "",
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A/B",
            "serial_number": "002"
		}
	
	]
};


response: 
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 29
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 29,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

#### read
```json

// 以參數查詢
// http:/192.168.249.112:3002/catalog/reels/findsearch?limit=2
// http://192.168.249.112:3002/catalog/reels/findsearch?reel_number=reel_number0001
// http://192.168.249.112:3002/catalog/reels/findsearch?reel_number=reel_number0001&order=des
// method: get
// body: none

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.reels findSearch successfully.",
    "data": [
        {
            "id": 31,
            "reel_number": "reel_number0001",
            "name": "reel_number0001",
            "full_name": "reel_number0001",
            "status": 5,
            "comment": "reel_number0001"
        },
        {
            "id": 29,
            "reel_number": "reel_number0001",
            "name": "reel_number0001",
            "full_name": "reel_number0001",
            "status": 5,
            "comment": "reel_number0001"
        }
    ]
}

```
#### update
```json
// http://192.168.249.104:3002/catalog/reels/creates
// method: post
// body:
{
    // "eventname":"reels",    // eventname (optional)
	"data": [                  // data array (required)
		{
            "reel_number": "D0800X001",
            "tax_id": "",
            "reserved": "",
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A/B",
            "serial_number": "001"
		},
		{
            "reel_number": "D0800X002",
            "tax_id": "",
            "reserved": "",
            "axis_code": "D(周轉軸)",
            "axis_size": "0800",
            "axis_surface": "A/B",
            "serial_number": "002"
		}
	
	]
};


response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.reels update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}

```
#### detete
```json

// http://192.168.249.104:3002/catalog/reels/deleteIDs
// method: delete
// body:
{
  "ids": ["5","15"]      // ids array (required)
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}
```

### tag_identifier API
#### create
```json

// http://192.168.249.112:3002/catalog/tag_identifier/creates
// method: post
// body:
{
  // "eventname":"tag_identifier",  // eventname (optional)
  "data": [                    // data json array (required)
        {
            //  "id": 1,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E2801170200003A6FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        },
        {
            //  "id": 2,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E280117020001346FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        }
  ]
};


response: 
{
  service: 1,
  code: 200,
  msg: { insertId: 27 },
  data: [
    {
      fieldCount: 0,
      affectedRows: 4,
      insertId: 27,
      serverStatus: 2,
      warningCount: 0,
      message: '&Records: 4  Duplicates: 0  Warnings: 0',
      protocol41: true,
      changedRows: 0
    }
  ]
}



```
#### read
```json
// http://192.168.123.183:3002/catalog/tag_identifier/findsearch?limit=2
// method: get
// body: none

response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.tag_identifier findSearch successfully.",
    "data": [
        {
            "id": 1,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E2801170200003A6FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        },
        {
            "id": 2,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E280117020001346FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        }
    ]
}


```
#### update
```json

// http://192.168.123.183:3002/catalog/tag_identifier/creates
// method: post
// body:
{
  "eventname":"tag_identifier",    // eventname (optional)
  "data": [                    // data json array (required)
        {
            //  "id": 1,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E2801170200003A6FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        },
        {
            //  "id": 2,
            "reel_number": "D0800X001",
            "name": "440800A001",
            "uid": null,
            "tid": "E280117020001346FACD0B5F",
            "epc": "35412204000000440800A001",
            "axis_surface": "A",
            "status": null,
            "reserved": null
        }
  ]
};


response: 
{
  service: 1,
  code: 200,
  msg: { insertId: 27 },
  data: [
    {
      fieldCount: 0,
      affectedRows: 4,
      insertId: 27,
      serverStatus: 2,
      warningCount: 0,
      message: '&Records: 4  Duplicates: 0  Warnings: 0',
      protocol41: true,
      changedRows: 0
    }
  ]
}


```
#### detete
```json

// http://192.168.249.104:3002/catalog/tag_identifier/deleteIDs
// method: delete
// body:
{
  "ids": ["5","15"]      // ids array (required)
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

### virtual_devices API
#### create
```json
// http://192.168.249.112:3002/catalog/virdevice/creates
// method: post
// body:

{
	"data": [                  
		{
            "id": 1,
            "name": "BS01G01RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 2,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:38:33.000"
		},
		{
            "id": 2,
            "name": "BS01G02RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 1,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:39:00.000"
		}
	]
}

response: 
{
  service: 1,
  code: 200,
  msg: { insertId: 27 },
  data: [
    {
      fieldCount: 0,
      affectedRows: 4,
      insertId: 27,
      serverStatus: 2,
      warningCount: 0,
      message: '&Records: 4  Duplicates: 0  Warnings: 0',
      protocol41: true,
      changedRows: 0
    }
  ]
}


```
#### read
```json

// http://192.168.249.104:3002/catalog/virdevice/findsearch?limit=20
// method: get
// body: none


response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.virtual_devices findSearch successfully.",
    "data": [
        {
            "id": 1,
            "name": "BS01G01RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 2,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:38:33.000"
        },
        {
            "id": 2,
            "name": "BS01G02RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 1,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:39:00.000"
        }
    ]
}


```
#### update
```json
{
	"data": [                  
		{
            "id": 1,
            "name": "BS01G01RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 2,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:38:33.000"
		},
		{
            "id": 2,
            "name": "BS01G02RF01",
            "full_name": "給線滿軸準備區(1100)",
            "type": 1100,
            "device_ip": "192.167.51.51",
            "antennas": 1,
            "location_id": null,
            "machine_id": 1,
            "groupst": null,
            "status": null,
            "parent_id": null,
            "comment": "給線滿軸準備區(1100)",
            "created": "2023-10-24 13:20:52.000",
            "updated": "2023-10-26 08:39:00.000"
		}
	]
}


response: 
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.virtual_devices update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}


```
#### detete
```json

// 批次刪除資料
// http://192.168.249.104:3002/catalog/virdevice/deleteIDs
// method: delete
// body:
{
  // "eventname":"virtual_devices",  // eventname (optional)
  "ids": ["5","15"]      // ids array (required)
}

// resp:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

### orders API
#### create
```json

// http://192.168.249.104:3002/catalog/orders/creates
// method: post
// body:
{
  // "eventname":"eventtest02", // eventname (optional)
  "data": [                    // data json array (required)
           {
		        "order_number": "123456789",
		        "reel_number": "RN123456789",
		        "name": "RN123456789",
		        "full_name": "RN123456789",
		        "type": 0,
		        "created": "2023-08-15 03:15:54.000",
		        "updated": "2023-08-15 03:15:54.000",
		        "status": 1,
		        "comment": "RN123456789"
            },
            {
		        "order_number": "123654789",
		        "reel_number": "RN123654789",
		        "name": "RN123654789",
		        "full_name": "RN123654789",
		        "type": 0,
		        "created": "2023-08-15 03:15:54.000",
		        "updated": "2023-08-15 03:15:54.000",
		        "status": 1,
		        "comment": "RN123654789"
            }
  ]
};


response: 
{
  service: 1,
  code: 200,
  msg: { insertId: 27 },
  data: [
    {
      fieldCount: 0,
      affectedRows: 2,
      insertId: 27,
      serverStatus: 2,
      warningCount: 0,
      message: '&Records: 2  Duplicates: 0  Warnings: 0',
      protocol41: true,
      changedRows: 0
    }
  ]
}
```
#### read
```json
// http://192.168.249.112:3002/catalog/orders/findsearch?limit=2
// method: get
// body: none

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.workorders findSearch successfully.",
    "data": [
        {
            "id": 1,
            "order_number": "123456789",
            "reel_number": "RN123456789",
            "name": "RN123456789",
            "full_name": "RN123456789",
            "type": 0,
            "created": "2023-08-15 03:15:54.000",
            "updated": "2023-08-15 03:15:54.000",
            "status": 1,
            "comment": "RN123456789"
        },
        {
            "id": 2,
            "order_number": "123654788",
            "reel_number": "RN123654788",
            "name": "RN123654789",
            "full_name": "RN123654788",
            "type": 0,
            "created": "2023-08-15 03:15:54.000",
            "updated": "2023-08-15 03:15:54.000",
            "status": 1,
            "comment": "RN123654788"
        }
    ]
}



```
#### update
```json
// http://192.168.249.112:3002/catalog/orders/creates
// method: post
// body:
{
	"data": [                  
		{
            "id": 1,
            "order_number": "123456789",
            "reel_number": "RN123456789",
            "name": "RN123456789",
            "full_name": "RN123456789",
            "type": 0,
            "created": "2023-08-15 03:15:54.000",
            "updated": "2023-08-15 03:15:54.000",
            "status": 1,
            "comment": "RN123456789"
		},
		{
            "id": 2,
            "order_number": "123654788",
            "reel_number": "RN123654788",
            "name": "RN123654789",
            "full_name": "RN123654788",
            "type": 0,
            "created": "2023-08-15 03:15:54.000",
            "updated": "2023-08-15 03:15:54.000",
            "status": 1,
            "comment": "RN123654788"
		}
	]
}

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.workorders update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}

```
#### detete
```json

// http://192.168.249.104:3002/catalog/orders/deleteIDs
// method: delete
// body:
{

  // "eventname":"eventtest02",  // eventname (optional)
  "ids": ["5","15"]      // ids array (required)
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

### data_analyzer_log API
#### create
```json
// http://192.168.249.123:3002/catalog/data_analyzer/analyzer_log
// method: post
// body:
{
    "data": [
       {
            // "id": 1,
            "device_ip": "192.168.23.253",
            "antenna": 2,
            "epc": "202003180000000000000005",
            "tids": "2e801105200078c1c0440a51, 2e801105200078c1c0440a52, 2e801105200078c1c0440a53",
            "first_appearance_time": "2023-09-20 08:11:31.000",
            "last_appearance_time": "2023-09-20 08:11:31.000",
            "packets": null,
            "first_time": "2023-09-20 08:11:31.000",
            "last_time": "2023-09-20 08:11:31.000",
            "count": null,
            "max_rssi": null,
            "min_rssi": null,
            "last_rssi": null,
            "total_count_rssi": null,
            "threadshold": null,
            "chosen": false,
            "axial_surface": "A"            
        },
        {
            // "id": 2,
            "device_ip": "192.168.24.36",
            "antenna": 1,
            "epc": "202003180000000000000006",
            "tids": "2e801105200078c1c0440a51, 2e801105200078c1c0440a52, 2e801105200078c1c0440a53",
            "first_appearance_time": "2023-09-20 08:11:31.000",
            "last_appearance_time": "2023-09-20 08:11:31.000",
            "packets": null,
            "first_time": "2023-09-20 08:11:31.000",
            "last_time": "2023-09-20 08:11:31.000",
            "count": null,
            "max_rssi": null,
            "min_rssi": null,
            "last_rssi": null,
            "total_count_rssi": null,
            "threadshold": null,
            "chosen": true,
            "axial_surface": "B"                
        }

    ]
}

response:


```
#### read
```json
// http://192.168.249.112:3002/catalog/data_analyzer/analyzer_log?limit=2
// method: get
// body: none

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_data.data_analyzer_log findSearch successfully.",
    "data": [
        {
            "id": 4120,
            "device_ip": "192.168.249.53",
            "antenna": 1,
            "epc": "AAAAAAAAAAAAAA12300000E1",
            "tids": "{\"tid\":\"E28011052000768EA6920A68\",\"count\":4,\"packets\":4,\"timestamp\":{\"first_time\":1606450264532,\"last_time\":1606450264532},\"rssi\":{\"max\":-50.8,\"min\":-51.8,\"last\":-51.8,\"total\":-206},\"last_time\":1606450271764}",
            "first_appearance_time": "2023-11-29 18:18:57.000",
            "last_appearance_time": "2023-11-29 18:18:57.000",
            "packets": 4,
            "first_time": "2020-11-27 12:11:04.000",
            "last_time": "2020-11-27 12:11:11.000",
            "count": 4,
            "max_rssi": -50.8,
            "min_rssi": -51.8,
            "last_rssi": -51.8,
            "total_count_rssi": -206,
            "threadshold": null,
            "chosen": null,
            "axial_surface": null
        },
        {
            "id": 4119,
            "device_ip": "192.168.249.53",
            "antenna": 1,
            "epc": "AAAAAAAAAAAAAAFB300000CB",
            "tids": "{\"tid\":\"E28011052000750EA69B0A68\",\"count\":117,\"packets\":117,\"timestamp\":{\"first_time\":1606450263283,\"last_time\":1606450263283},\"rssi\":{\"max\":-47.6,\"min\":-61.1,\"last\":-48.3,\"total\":-6184},\"last_time\":1606450273140}",
            "first_appearance_time": "2023-11-29 18:18:57.000",
            "last_appearance_time": "2023-11-29 18:18:57.000",
            "packets": 117,
            "first_time": "2020-11-27 12:11:03.000",
            "last_time": "2020-11-27 12:11:13.000",
            "count": 117,
            "max_rssi": -47.6,
            "min_rssi": -61.1,
            "last_rssi": -48.3,
            "total_count_rssi": -6184,
            "threadshold": null,
            "chosen": null,
            "axial_surface": null
        }
    ]
}


```
#### update

```json
// http://192.168.249.112:3002/catalog/data_analyzer/analyzer_log
// method: post
// body:

{
	"data": [                  
        {
            // "id": 4120,
            "device_ip": "192.168.249.53",
            "antenna": 1,
            "epc": "AAAAAAAAAAAAAA12300000E1",
            "tids": "{\"tid\":\"E28011052000768EA6920A68\",\"count\":4,\"packets\":4,\"timestamp\":{\"first_time\":1606450264532,\"last_time\":1606450264532},\"rssi\":{\"max\":-50.8,\"min\":-51.8,\"last\":-51.8,\"total\":-206},\"last_time\":1606450271764}",
            "first_appearance_time": "2023-11-29 18:18:57.000",
            "last_appearance_time": "2023-11-29 18:18:57.000",
            "packets": 4,
            "first_time": "2020-11-27 12:11:04.000",
            "last_time": "2020-11-27 12:11:11.000",
            "count": 4,
            "max_rssi": -50.8,
            "min_rssi": -51.8,
            "last_rssi": -51.8,
            "total_count_rssi": -206,
            "threadshold": null,
            "chosen": null,
            "axial_surface": null
        },
        {
            // "id": 4119,
            "device_ip": "192.168.249.53",
            "antenna": 1,
            "epc": "AAAAAAAAAAAAAAFB300000CB",
            "tids": "{\"tid\":\"E28011052000750EA69B0A68\",\"count\":117,\"packets\":117,\"timestamp\":{\"first_time\":1606450263283,\"last_time\":1606450263283},\"rssi\":{\"max\":-47.6,\"min\":-61.1,\"last\":-48.3,\"total\":-6184},\"last_time\":1606450273140}",
            "first_appearance_time": "2023-11-29 18:18:57.000",
            "last_appearance_time": "2023-11-29 18:18:57.000",
            "packets": 117,
            "first_time": "2020-11-27 12:11:03.000",
            "last_time": "2020-11-27 12:11:13.000",
            "count": 117,
            "max_rssi": -47.6,
            "min_rssi": -61.1,
            "last_rssi": -48.3,
            "total_count_rssi": -6184,
            "threadshold": null,
            "chosen": null,
            "axial_surface": null
        }
	]
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_data.data_analyzer_log update successfully.",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 2  Warnings: 0",
            "protocol41": true,
            "changedRows": 2
        }
    ]
}


```
#### detete
```json

// http://192.168.249.104:3002/catalog/data_analyzer/deleteIDs
// method: delete
// body:
{
  "ids": ["5","15"]      
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}


```

### reader_device_state API
#### create
```json
// http://192.168.249.104:3002/catalog/peripheral/device_state
// Method: POST
// body:
{
    "data": [
        {
            // "id": 15,
            "app_id": 2,
            "device_ip": "device003",
            "device_port": null,
            "device_mac": null,
            "related_id": null,
            "name": "device005",
            "state": null,
            "link": null,
            "work": null,
            "driver": "device001",
            "action_cmd": 1,
            "actual_state": null,
            "temperature_pa": null,
            "temperature_rf": 0,
            "temperature_sys": 0,
            "connected": "2023-08-28 06:22:54.000",
            "updated": "2023-08-26 06:22:54.000",
            "received": "2023-09-06 19:14:36.015",
            "rawdata": "rawdata test"
        },

        {
            // "id": 1,
            "app_id": 2,
            "device_ip": "device005",
            "device_port": null,
            "device_mac": null,
            "related_id": null,
            "name": "device005",
            "state": null,
            "link": null,
            "work": null,
            "driver": "device001",
            "action_cmd": null,
            "actual_state": null,
            "temperature_pa": null,
            "temperature_rf": 0,
            "temperature_sys": 0,
            "connected": "2023-08-28 06:22:54.000",
            "updated": "2023-08-26 06:22:54.000",
            "received": "2023-09-06 19:14:36.015",
            "rawdata": "rawdata test"
        }
    ]
}

response:
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 9
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 9,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}


```
#### read
```json
// http://192.168.249.112:3002/catalog/peripheral/device_state?limit=2
// Method: GET
// body:
response:


{
    "service": 1,
    "code": 200,
    "msg": "walsin_peripheral.reader_device_state findSearch successfully.",
    "data": [
        {
            "id": 87,
            "app_id": "1f043694-e510-4513-ba7d-3351fed66401",
            "device_ip": "192.168.19.31",
            "device_port": "11001",
            "device_mac": "00:0c:43:e1:76:2a",
            "related_id": "5b7104df-279b-4cc3-a970-71417ca261da",
            "name": "",
            "antennas": null,
            "state": 0,
            "link": null,
            "work": null,
            "driver": "",
            "action_cmd": -1,
            "actual_state": -1,
            "temperature_pa": 48,
            "temperature_rf": 48,
            "temperature_sys": 48,
            "connected": "2023-09-08 17:12:47.000",
            "updated": "2023-09-26 05:43:18.000",
            "received": "2023-09-08 17:12:47.000",
            "rawdata": "{\"antenna\":1,\"command\":240,\"commandstring\":\"keepalivestring\",\"count\":0,\"count_rssi\":0,\"count_start\":1692695844,\"count_transfer\":1692695855,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"000000000000000000000000\",\"max_rssi\":0,\"min_rssi\":0,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":0,\"temperature_pa\":\"48\",\"temperature_rf\":\"48\",\"temperature_sys\":\"48\",\"tid\":\"0000000000\",\"timestamp\":1692695844,\"usr\":\"\",\"version\":10000}"
        },
        {
            "id": 91,
            "app_id": "294b2ac1-ce5b-4961-afa4-a2d98a8aba09",
            "device_ip": "192.168.249.51",
            "device_port": "59324",
            "device_mac": "14:00:8b:c2:27:f9",
            "related_id": "2b865fac-5aec-4933-8485-dec84db8bd91",
            "name": "",
            "antennas": null,
            "state": 0,
            "link": null,
            "work": 22,
            "driver": "",
            "action_cmd": -1,
            "actual_state": -1,
            "temperature_pa": 47,
            "temperature_rf": 56,
            "temperature_sys": 56,
            "connected": "2023-11-29 17:53:44.000",
            "updated": "2023-11-29 17:53:44.000",
            "received": "2023-11-29 17:53:44.733",
            "rawdata": "{\"antenna\":1,\"command\":240,\"commandstring\":\"keepalivestring\",\"count\":1,\"count_gap\":9999,\"count_rssi\":0,\"count_start\":1606449019000,\"count_transfer\":1606449030000,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.51\",\"device_mac\":\"14:00:8b:c2:27:f9\",\"epc\":\"0000000000000000000000000\",\"interval\":9999,\"max_rssi\":-2000,\"min_rssi\":-2000,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-2000,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":0,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"temperature_pa\":\"47\",\"temperature_rf\":\"56\",\"temperature_sys\":\"56\",\"tid\":\"0000000000000000000000000\",\"timestamp\":1606449019000,\"usr\":\"\",\"version\":10002}"
        }
    ]
}

```
#### update
同 create
```json
// http://192.168.249.112:3002/catalog/peripheral/device_state
// Method: POST
// body:
{
    "data": [
        {
            "id": 87,
            "app_id": "1f043694-e510-4513-ba7d-3351fed66401",
            "device_ip": "192.168.19.31",
            "device_port": "11001",
            "device_mac": "00:0c:43:e1:76:2a",
            "related_id": "5b7104df-279b-4cc3-a970-71417ca261da",
            "name": "",
            "antennas": null,
            "state": 0,
            "link": null,
            "work": null,
            "driver": "",
            "action_cmd": -1,
            "actual_state": -1,
            "temperature_pa": 48,
            "temperature_rf": 48,
            "temperature_sys": 48,
            "connected": "2023-09-08 17:12:47.000",
            "updated": "2023-09-26 05:43:18.000",
            "received": "2023-09-08 17:12:47.000",
            "rawdata": "{\"antenna\":1,\"command\":240,\"commandstring\":\"keepalivestring\",\"count\":0,\"count_rssi\":0,\"count_start\":1692695844,\"count_transfer\":1692695855,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"000000000000000000000000\",\"max_rssi\":0,\"min_rssi\":0,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":0,\"temperature_pa\":\"48\",\"temperature_rf\":\"48\",\"temperature_sys\":\"48\",\"tid\":\"0000000000\",\"timestamp\":1692695844,\"usr\":\"\",\"version\":10000}"
        },
        {
            "id": 91,
            "app_id": "294b2ac1-ce5b-4961-afa4-a2d98a8aba09",
            "device_ip": "192.168.249.51",
            "device_port": "59324",
            "device_mac": "14:00:8b:c2:27:f9",
            "related_id": "2b865fac-5aec-4933-8485-dec84db8bd91",
            "name": "",
            "antennas": null,
            "state": 0,
            "link": null,
            "work": 22,
            "driver": "",
            "action_cmd": -1,
            "actual_state": -1,
            "temperature_pa": 47,
            "temperature_rf": 56,
            "temperature_sys": 56,
            "connected": "2023-11-29 17:53:44.000",
            "updated": "2023-11-29 17:53:44.000",
            "received": "2023-11-29 17:53:44.733",
            "rawdata": "{\"antenna\":1,\"command\":240,\"commandstring\":\"keepalivestring\",\"count\":1,\"count_gap\":9999,\"count_rssi\":0,\"count_start\":1606449019000,\"count_transfer\":1606449030000,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.51\",\"device_mac\":\"14:00:8b:c2:27:f9\",\"epc\":\"0000000000000000000000000\",\"interval\":9999,\"max_rssi\":-2000,\"min_rssi\":-2000,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-2000,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":0,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"temperature_pa\":\"47\",\"temperature_rf\":\"56\",\"temperature_sys\":\"56\",\"tid\":\"0000000000000000000000000\",\"timestamp\":1606449019000,\"usr\":\"\",\"version\":10002}"
        }
    ]
}

response:


{
    "service": 1,
    "code": 200,
    "msg": "tag_reporting creates or update successfully.",
    "data": {
        "fieldCount": 0,
        "affectedRows": 2,
        "insertId": 0,
        "serverStatus": 2,
        "warningCount": 0,
        "message": "&Records: 2  Duplicates: 2  Warnings: 0",
        "protocol41": true,
        "changedRows": 2
    }
}

```
#### detete
```json
// http://192.168.249.104:3002/catalog/peripheral/device_delete_ids
// method: delete
// body:
{
  "ids": ["5","15"]      
}


response:
{
    "service": 1,
    "code": 200,
    "msg": "delete successfully ids: ('5','15')",
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 0,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}


```

### tag_reporting API
#### create
```json
// http://192.168.249.112:3002/catalog/peripheral/tag_reporting
// method: post
// body:
{
    "data": [
        {
            "seqno": 94827,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -56,
            "count_start": "2020-11-27 12:11:17.756",
            "count_transfer": "2020-11-27 12:11:17.756",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000750EA69B0A68",
            "epc": "AAAAAAAAAAAAAAFB300000CB",
            "max_rssi": -55.9,
            "min_rssi": -55.9,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -55.9,
            "timestamp": "2020-11-27 12:11:17.753",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.211Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-56,\"count_start\":1606450277756,\"count_transfer\":1606450277756,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFB300000CB\",\"interval\":1000,\"max_rssi\":-5590,\"min_rssi\":-5590,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-5590,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94827,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000750EA69B0A68\",\"timestamp\":1606450277753,\"usr\":\"\",\"version\":10002}"
        },
        {
            "seqno": 94826,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -25,
            "count_start": "2020-11-27 12:11:17.735",
            "count_transfer": "2020-11-27 12:11:17.735",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000784EA6A20A68",
            "epc": "AAAAAAAAAAAAAAFE300000CE",
            "max_rssi": -25,
            "min_rssi": -25,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -25,
            "timestamp": "2020-11-27 12:11:17.728",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.210Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-25,\"count_start\":1606450277735,\"count_transfer\":1606450277735,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFE300000CE\",\"interval\":1000,\"max_rssi\":-2500,\"min_rssi\":-2500,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-2500,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94826,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000784EA6A20A68\",\"timestamp\":1606450277728,\"usr\":\"\",\"version\":10002}"
        }
    ]
}

response:
{
    "service": 1,
    "code": 200,
    "msg": {
        "insertId": 1452217
    },
    "data": [
        {
            "fieldCount": 0,
            "affectedRows": 2,
            "insertId": 1452217,
            "serverStatus": 2,
            "warningCount": 0,
            "message": "&Records: 2  Duplicates: 0  Warnings: 0",
            "protocol41": true,
            "changedRows": 0
        }
    ]
}

```

#### read
```json
// http://192.168.249.112:3002/catalog/peripheral/tag_findsearch?limit=2
// method: get
// body: none

response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_peripheral.tag_reporting  findSearch successfully.",
    "data": [
        {
            "sn": 1452216,
            "seqno": 94827,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -56,
            "count_start": "2020-11-27 12:11:17.756",
            "count_transfer": "2020-11-27 12:11:17.756",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000750EA69B0A68",
            "epc": "AAAAAAAAAAAAAAFB300000CB",
            "max_rssi": -55.9,
            "min_rssi": -55.9,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -55.9,
            "timestamp": "2020-11-27 12:11:17.753",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.211Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-56,\"count_start\":1606450277756,\"count_transfer\":1606450277756,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFB300000CB\",\"interval\":1000,\"max_rssi\":-5590,\"min_rssi\":-5590,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-5590,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94827,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000750EA69B0A68\",\"timestamp\":1606450277753,\"usr\":\"\",\"version\":10002}"
        },
        {
            "sn": 1452215,
            "seqno": 94826,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -25,
            "count_start": "2020-11-27 12:11:17.735",
            "count_transfer": "2020-11-27 12:11:17.735",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000784EA6A20A68",
            "epc": "AAAAAAAAAAAAAAFE300000CE",
            "max_rssi": -25,
            "min_rssi": -25,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -25,
            "timestamp": "2020-11-27 12:11:17.728",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.210Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-25,\"count_start\":1606450277735,\"count_transfer\":1606450277735,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFE300000CE\",\"interval\":1000,\"max_rssi\":-2500,\"min_rssi\":-2500,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-2500,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94826,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000784EA6A20A68\",\"timestamp\":1606450277728,\"usr\":\"\",\"version\":10002}"
        }
    ]
}


```
#### update
```json
http://192.168.123.183:3002/catalog/peripheral/tag_reporting
// method: post
// body:
{
    "data": [
        {
            "sn": 1452216,
            "seqno": 94827,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -56,
            "count_start": "2020-11-27 12:11:17.756",
            "count_transfer": "2020-11-27 12:11:17.756",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000750EA69B0A68",
            "epc": "AAAAAAAAAAAAAAFB300000CB",
            "max_rssi": -55.9,
            "min_rssi": -55.9,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -55.9,
            "timestamp": "2020-11-27 12:11:17.753",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.211Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-56,\"count_start\":1606450277756,\"count_transfer\":1606450277756,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFB300000CB\",\"interval\":1000,\"max_rssi\":-5590,\"min_rssi\":-5590,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-5590,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94827,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000750EA69B0A68\",\"timestamp\":1606450277753,\"usr\":\"\",\"version\":10002}"
        },
        {
            "sn": 1452215,
            "seqno": 94826,
            "antenna": 1,
            "command": 35,
            "commandstring": "URC_CMD_RFID_Inven_Receive",
            "count": 1,
            "count_rssi": -25,
            "count_start": "2020-11-27 12:11:17.735",
            "count_transfer": "2020-11-27 12:11:17.735",
            "device_ip": "192.168.249.53",
            "device_mac": "44:00:21:25:2c:aa",
            "tid": "E28011052000784EA6A20A68",
            "epc": "AAAAAAAAAAAAAAFE300000CE",
            "max_rssi": -25,
            "min_rssi": -25,
            "offline_data": 0,
            "operation": "0",
            "output_mode": "1",
            "rssi": -25,
            "timestamp": "2020-11-27 12:11:17.728",
            "usr": "",
            "version": "10002",
            "received": "2023-11-29T18:19:02.210Z",
            "count_gap": 9999,
            "interval_time": 1000,
            "rssi_average": -20,
            "rssi_median": -20,
            "speed_average": 9999,
            "speed_gap": 9999,
            "speed_median": 9999,
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_gap\":9999,\"count_rssi\":-25,\"count_start\":1606450277735,\"count_transfer\":1606450277735,\"device_id\":\"arizon-rfid\",\"device_ip\":\"192.168.249.53\",\"device_mac\":\"44:00:21:25:2c:aa\",\"epc\":\"AAAAAAAAAAAAAAFE300000CE\",\"interval\":1000,\"max_rssi\":-2500,\"min_rssi\":-2500,\"offline_data\":false,\"operation\":0,\"output_mode\":1,\"rssi\":-2500,\"rssi_average\":-2000,\"rssi_median\":-2000,\"seqno\":94826,\"speed_average\":9999,\"speed_gap\":9999,\"speed_median\":9999,\"tid\":\"E28011052000784EA6A20A68\",\"timestamp\":1606450277728,\"usr\":\"\",\"version\":10002}"
        }
    ]
}

response:

{
    "service": 1,
    "code": 200,
    "msg": "walsin_peripheral.tag_reporting update successfully.",
    "data": {
        "fieldCount": 0,
        "affectedRows": 2,
        "insertId": 0,
        "serverStatus": 2,
        "warningCount": 0,
        "message": "&Records: 2  Duplicates: 2  Warnings: 0",
        "protocol41": true,
        "changedRows": 2
    }
}


```
#### detete
```json
// http://192.168.249.123:3002/catalog/peripheral/tag_reporting?day=10&count=100000
// method: delete
// body: none

response:
{
    "service": 1,
    "code": 200,
    "msg": "delete 30 days ago successfully.",
    "data": [
        []
    ]
}

```



# 系統整合安裝與啟動
## 系統安裝
1. 安裝xampp-windows-x64-8.2.4-0-VS16-installer.exe
2. 設定xampp的apache/MySQL的 帳號(root)/密碼(q45101524Q)
3. 安裝node-v18.18.2-x64.msi
4. 安裝 pm2 command : npm i pm2 -g 

## 資料庫安裝設定
1. 登入網址: http://127.0.0.1/phpMyAdmin/index.php
2. 以手動方式新增資料庫名稱: walsin_data, walsin_master, walsin_peripheral
3.  以手動方式匯入資料庫檔案: 
	目錄: \walsin\idts-data\node_app\dbsql\
	walsin_data_2023-11-27-11-24-14.sql
	walsin_master_2023-11-27-11-24-14.sql
	walsin_peripheral_2023-11-27-11-24-14.sql
4.  亦可以使用 資料庫備份/回復方式，設定參數操作:


## IDTS安裝設定
1. 複製檔案到目錄中: walsin
2. idts-core_20231122.zip, idts-data_20231122.zip, idts-ui_20231123.zip, idts_update.bat
3. 執行解壓縮程式: run idts_update.bat

## PM2安裝設定
1. idts-data: pm2 start -n idts-data ./dist-server/bin/www.js -i 3 --watch
2. idts-ui: pm2 start -n idts-ui ./node_modules/next/dist/bin/next -i 1 --watch -- start
3. idts-core: pm2 start -n idts-data ./dist-server/bin/www.js -i 3 --watch
4. 安裝window service: npm install -g pm2-windows-service
5. 啟動服務: pm2-service-install -n PM2
6. 關閉服務: pm2-service-uninstall -n PM2  
7. 顯示狀態: pm2 status
8. 顯示CPU效能: pm2 monit
9. 顯示log: pm2 log



# 資料庫備份/回復

## 資料庫備份
1. 目錄: idts-data/node_app
	執行備份command: node dbbackup_cmd.js
2. 修改參數:
	edit: app.config.json
```json
  "dbconfig_walsin_master": {
    "HOST": "127.0.0.1",
    "USER": "root",
    "PASSWORD": "q45101524Q",
    "DB": "walsin_master",
    "PORT": "3306"
    }
```


3. 修改參數:
	edit: dbbackup_cmd.js
```json
	jsonData = {
        "eventname": "eventtest02",
        "data": [{
                "backupDBName": "walsin_data"
            },
            {
                "backupDBName": "walsin_master"
            },
            {
                "backupDBName": "walsin_peripheral",
            }
        ]
    }
```


## 資料庫回復
1. 目錄: idts-data/node_app
	執行備份command: node dbrestore_cmd.js
2. 修改參數:
	edit: app.config.json
```json
  "dbconfig_walsin_master": {
    "HOST": "127.0.0.1",
    "USER": "root",
    "PASSWORD": "q45101524Q",
    "DB": "walsin_master",
    "PORT": "3306"
    },
  "backup_config": {
    "BACKUP_DIR": "./dbsql",    // 預設path
    "BODYLIMIT": "100kb",
    "RESERVE1": "reserve1",
    "RESERVE2": "reserve2",
    "RESERVE3": "reserve3"
  }
```


3. 修改參數:
	edit: dbrestore_cmd.js
	```json
    jsonData = {
        "eventname": "eventtest02",
        "data": [{
                "restoreDBName": "walsin_data_20231127",
                "restoreFileName": "walsin_data_2023-11-27-11-24-14.sql"
            },
            {
                "restoreDBName": "walsin_master_20231127",
                "restoreFileName": "walsin_master_2023-11-27-11-24-14.sql"
            },
            {
                "restoreDBName": "walsin_peripheral_20231127",
                "restoreFileName": "walsin_peripheral_2023-11-27-11-24-14.sql"
            }
        ]
    }

	```


