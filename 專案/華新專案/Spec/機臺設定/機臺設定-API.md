# hmi_config api format

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


// 以參數查詢
// http://192.168.249.123:3002/catalog/hmi_configure/findsearch
// http://192.168.123.183:3002/catalog/hmi_configure/findsearch?limit=2
  // 可查詢欄位如下
  // static table_fields = {
  //            name: null,
  //            reel_id: null,
  //            full_name: null,
  //            address_ip: null,
  //            type: null,
  //            status: null,
  //            comment: null,
  // }

// method: get
// body: none

// response:
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

// http://192.168.249.104:3002/catalog/user/deleteIDs
// method: delete
// body:
{
  "eventname":"users",   // eventname (optional)
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

// 多表關聯查詢
// postman test ok
// 沒有參數則查詢全部顯示
// http://192.168.249.123:3002/catalog/hmi_configure/findReports
// 可以加入查詢條件 (optional query)
// http://192.168.249.123:3002/catalog/hmi_configure/findReports?limit=1000&offset=10&order=DESC

// 說明: tb1 = hmi_config, tb2 = machines, tb3 = virtual_devices 對應資料表名稱.
// 說明: tb1.id = hmi_config.id, tb2.id = machines.id, tb3.name = virtual_devices.name 對應資料表欄位名稱.
// http://192.168.249.123:3002/catalog/hmi_configure/findReports
// method: post
// body:
{
//   "eventname":"hmi_config"",  // table name (optional)
    "data":
        {
            // "tb1.id": "1",
            // "tb2.id": "1",
            // "tb3.name": "BS01G05RF01"
            // "tb2.id": "2"
            // "tb3.machine_id": "5"
            "tb2.machine_number": "CS05"
        }
}


// response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.hmi_config findSearch successfully.",
    "data": [
        {
            "hmi_id": 4,
            "hmi_name": "HMI004",
            "hmi_status": null,
            "tb2_id": 5,
            "tb2_machine_number": "CS05",
            "tb2_name": "1600弓絞C",
            "tb2_status": 0,
            "tb3_id": 31,
            "tb3_name": "CS05T01",
            "tb3_full_name": "捲取機(2300)",
            "tb3_status": null
        },
        {
            "hmi_id": 4,
            "hmi_name": "HMI004",
            "hmi_status": null,
            "tb2_id": 5,
            "tb2_machine_number": "CS05",
            "tb2_name": "1600弓絞C",
            "tb2_status": 0,
            "tb3_id": 32,
            "tb3_name": "CS05T01RF01",
            "tb3_full_name": "捲取滿軸準備區(2100)",
            "tb3_status": null
        },
        {
            "hmi_id": 4,
            "hmi_name": "HMI004",
            "hmi_status": null,
            "tb2_id": 5,
            "tb2_machine_number": "CS05",
            "tb2_name": "1600弓絞C",
            "tb2_status": 0,
            "tb3_id": 33,
            "tb3_name": "CS05T01RE01",
            "tb3_full_name": "捲取空軸準備區(2200)",
            "tb3_status": null
        }
    ]
}




```