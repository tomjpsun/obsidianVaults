```js

// http://192.168.123.183:3002/catalog/productions/creates
// method: post
// body:
{
//   "eventname":"productions",  // table name (optional)
    "data":[
        {
            "id": 1,
            "workorder_id": 2,
            "reel_number": "D0800X002",
            "virtual_devices_id": 2,
            "up_time": "2023-10-18 13:25:13.000",
            "down_time": "2023-10-18 13:28:13.000",
            "production_time": "2023-10-17 13:28:13.000",
            "average_line_speed": 78,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:32.000",
            "updated": "2023-09-04 01:56:32.000",
            "status": 0,
            "comment": "production0111"
        },
        {
            "id": 2,
            "workorder_id": 1,
            "reel_number": "D0800X009",
            "virtual_devices_id": 2,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 0,
            "comment": "production0222"
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
// http://192.168.123.183:3002/catalog/productions/findsearch
  // 可查詢欄位如下
  //      {
            // "workorder_id": null,
            // "reel_number": null,
            // "virtual_devices_id": null,
            //XXX "manufacturing_time": null,
            //XXX "effective_time": null,
            //XXX "created": "2023-09-16 08:34:03.000",
            // "updated": "2023-09-16 08:34:03.000",
            // "status": 0,
            // "comment": "productions0001"
  //      },

// method: get
// body: none

// response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.productions findSearch successfully.",
    "data": [
        {
            "id": 1,
            "workorder_id": 2,
            "reel_number": "D0800X002",
            "virtual_devices_id": 2,
            "up_time": "2023-10-18 13:25:13.000",
            "down_time": "2023-10-18 13:28:13.000",
            "production_time": "2023-10-17 13:28:13.000",
            "average_line_speed": 78,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:32.000",
            "updated": "2023-09-04 01:56:32.000",
            "status": 0,
            "comment": "production0111"
        },
        {
            "id": 2,
            "workorder_id": 1,
            "reel_number": "D0800X009",
            "virtual_devices_id": 2,
            "up_time": "2023-10-17 13:29:44.000",
            "down_time": "2023-10-16 13:29:44.000",
            "production_time": "2023-10-10 13:29:44.000",
            "average_line_speed": 543,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:35.000",
            "updated": "2023-09-04 01:56:35.000",
            "status": 0,
            "comment": "production0222"
        }
    ]
}


// http://192.168.249.104:3002/catalog/productions/deleteIDs
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





// postman test ok
// 沒有參數則查詢全部顯示
// http://192.168.249.123:3002/catalog/productions/findReports
// 可以加入查詢條件 (optional query)
// http://192.168.249.123:3002/catalog/productions/findReports?limit=1000&offset=10&order=DESC
// method: POST
// body:
{
//   "tablename":"virtual_devices",  // table name (optional)
    "data":
        {
            // "tb4.name": "BS01G05RF01"
            "tb1.reel_number": "D0800X002"
        }

}



// response:
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.productions findReports successfully.",
    "data": [
        {
            "id": 1,
            "workorder_id": 2,
            "reel_number": "D0800X002",
            "virtual_devices_id": 5,
            "up_time": "2023-10-18 13:25:13.000",
            "down_time": "2023-10-18 13:28:13.000",
            "production_time": "2023-10-17 13:28:13.000",
            "average_line_speed": 78,
            "real_time_meter": 0,
            "created": "2023-09-04 01:56:32.000",
            "updated": "2023-09-04 01:56:32.000",
            "status": 0,
            "comment": "production0111",
            "order_number": "123654789",
            "virtual_devices_name": "BS01G05RF01"
        }
    ]
}



```