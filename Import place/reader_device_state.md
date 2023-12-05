



reader_device_state table:

```sql
  

Table "walsin_peripheral"."reader_device_state" {

  "id" bigint [unique, pk, increment]

  "app_id" int  

  "device_ip" varchar(250)  

  "net_port" varchar(32)

  "device_mac"  varchar(250)

  "related_id"  har(250) // == HMI IP  (related equipment)

  "name" varchar(256)

  "state" int

  "link" int // off = 0, on = 1

  "work" int // 0: Not connected, 1: Connected , 2: Connection abnormal

  "driver" varchar(100)

  "action_cmd" int    // action command: inventorystart=1, inventorystop=0

  "actual_state" int  // unknown = -1,  stop=0, running = 1

  "temperature_pa" double

  "temperature_rf" double

  "temperature_sys" double

  "connected" timestamp [default: `current_timestamp()`]

  "updated" timestamp [default: `current_timestamp()`]

  "received" timestamp [default: `current_timestamp()`] // received time

  "rawdata" varchar(512)

}
```

```json
// http://192.168.249.104:3002/catalog/peripheral/device_state
// Method: POST
// body:
{
//   "tablename":"reader_device_state",  // table name (optional)
//   "ids": ["19","15"]      // ids array (required)
    "data": [
        {
            // "id": 15,
            "app_id": 2,
            "device_ip": "device003",
            "net_port": null,
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
            "net_port": null,
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

// resp:
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


// http://192.168.249.104:3002/catalog/peripheral/device_state
// http://192.168.249.104:3002/catalog/peripheral/device_state?table=reader_device_state  // table name (optional)
// http://192.168.249.104:3002/catalog/peripheral/device_state?table=reader_device_state&limit=2 (optional)
// http://192.168.249.104:3002/catalog/peripheral/device_state?table=reader_device_state&order=desc&limit=2
// http://192.168.249.104:3002/catalog/peripheral/device_state
// Method: GET
// body:

// resp:
{

    "service": 1,

    "code": 200,

    "msg": "walsin_peripheral.reader_device_state findSearch successfully.",

    "data": [

        {

            "id": 15,

            "app_id": 2,

            "device_ip": "device003",

            "net_port": null,

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

            "id": 1,

            "app_id": 2,

            "device_ip": "device005",

            "net_port": null,

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

        },

        {

            "id": 2,

            "app_id": 3,

            "device_ip": "device006",

            "net_port": null,

            "device_mac": null,

            "related_id": null,

            "name": "device006",

            "state": null,

            "link": null,

            "work": null,

            "driver": "device001",

            "action_cmd": 2,

            "actual_state": null,

            "temperature_pa": null,

            "temperature_rf": 0,

            "temperature_sys": 0,

            "connected": "2023-08-28 06:22:54.000",

            "updated": "2023-09-06 03:58:28.000",

            "received": "2023-09-07 03:14:36.015",

            "rawdata": ""

        },

        {

            "id": 27,

            "app_id": 2,

            "device_ip": "device007",

            "net_port": null,

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

            "updated": "2023-09-07 02:36:49.000",

            "received": "2023-09-07 03:14:36.015",

            "rawdata": ""

        },

        {

            "id": 28,

            "app_id": 3,

            "device_ip": "device008",

            "net_port": null,

            "device_mac": null,

            "related_id": null,

            "name": "device006",

            "state": null,

            "link": null,

            "work": null,

            "driver": "device001",

            "action_cmd": 2,

            "actual_state": null,

            "temperature_pa": null,

            "temperature_rf": 0,

            "temperature_sys": 0,

            "connected": "2023-08-28 06:22:54.000",

            "updated": "2023-09-07 02:36:49.000",

            "received": "2023-09-07 03:14:36.015",

            "rawdata": ""

        }

    ]

}

// http://192.168.123.183:3002/catalog/peripheral/device_state
// Method: delete
// body:
{
  "table":"reader_device_state",  // table name (optional)
  "ids": ["5","15"]
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

lowdatalog table:
```json
// http://192.168.249.104:3002/catalog/peripheral/lowdatalog
// Method: POST
// body:
{
//   "tablename":"lowdatalog",  // table name (optional)
//   "ids": ["19","15"]      // ids array (required)
    "data": [
        {
            // "id": 1,
            "type": 2,
            "name": "00:0c:43:e1:76:2a",
            "created": "2023-09-07 02:45:00.000",
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_rssi\":-59,\"count_start\":1692695905,\"count_transfer\":1692695905,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"00B07A14B4584D91500000CE\",\"max_rssi\":-5840,\"min_rssi\":-5840,\"offline_data\":true,\"operation\":0,\"output_mode\":1,\"rssi\":-5840,\"tid\":\"E280119020006F7E4956031C\",\"timestamp\":1693221678,\"usr\":\"\",\"version\":10000}"
        },
        {
            // "id": 2,
            "type": 2,
            "name": "00:0c:43:e1:76:2a",
            "created": "2023-09-07 02:45:00.000",
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_rssi\":-59,\"count_start\":1692695905,\"count_transfer\":1692695905,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"00B07A14B4584D91500000CE\",\"max_rssi\":-5840,\"min_rssi\":-5840,\"offline_data\":true,\"operation\":0,\"output_mode\":1,\"rssi\":-5840,\"tid\":\"E280119020006F7E4956031C\",\"timestamp\":1693221678,\"usr\":\"\",\"version\":10000}"
        }
    ]
}


// http://192.168.249.104:3002/catalog/peripheral/lowdatalog
// Method: GET

{

    "service": 1,
    "code": 200,
    "msg": "walsin_peripheral.lowdatalog findSearch successfully.",
    "data": [
        {
            "id": 1,
            "type": 2,
            "name": "00:0c:43:e1:76:2a",
            "created": "2023-09-07 02:45:00.000",
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_rssi\":-59,\"count_start\":1692695905,\"count_transfer\":1692695905,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"00B07A14B4584D91500000CE\",\"max_rssi\":-5840,\"min_rssi\":-5840,\"offline_data\":true,\"operation\":0,\"output_mode\":1,\"rssi\":-5840,\"tid\":\"E280119020006F7E4956031C\",\"timestamp\":1693221678,\"usr\":\"\",\"version\":10000}"
        },
        {
            "id": 2,
            "type": 2,
            "name": "00:0c:43:e1:76:2a",
            "created": "2023-09-07 02:45:00.000",
            "rawdata": "{\"antenna\":1,\"command\":35,\"commandstring\":\"URC_CMD_RFID_Inven_Receive\",\"count\":1,\"count_rssi\":-59,\"count_start\":1692695905,\"count_transfer\":1692695905,\"device_ip\":\"192.168.19.31\",\"device_mac\":\"00:0c:43:e1:76:2a\",\"epc\":\"00B07A14B4584D91500000CE\",\"max_rssi\":-5840,\"min_rssi\":-5840,\"offline_data\":true,\"operation\":0,\"output_mode\":1,\"rssi\":-5840,\"tid\":\"E280119020006F7E4956031C\",\"timestamp\":1693221678,\"usr\":\"\",\"version\":10000}"
        }
    ]
}
```