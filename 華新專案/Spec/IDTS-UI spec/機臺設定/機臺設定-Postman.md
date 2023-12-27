POST http://192.168.249.112:3002/catalog/hmi_configure/findReports

Body 設定爲 Raw - JSON 
```js
{
    "data": 
        {
            "tb1.id": "1"
            // "tb2.id": "1"
            // "tb3.name": "BS01G05RF01"
            // "tb2.id": "2"
            // "tb3.machine_id": "5"
            // "tb2.machine_number": "CS05"
        }
}

```


response
```js
{
    "service": 1,
    "code": 200,
    "msg": "walsin_master.hmi_config findSearch successfully.",
    "data": [
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 1,
            "tb3_name": "BS01G01RF01",
            "tb3_full_name": "給線滿軸準備區(1100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 2,
            "tb3_name": "BS01G02RF01",
            "tb3_full_name": "給線滿軸準備區(1100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 3,
            "tb3_name": "BS01G03RF01",
            "tb3_full_name": "給線滿軸準備區(1100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 4,
            "tb3_name": "BS01G04RF01",
            "tb3_full_name": "給線滿軸準備區(1100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 5,
            "tb3_name": "BS01G05RF01",
            "tb3_full_name": "給線滿軸準備區(1100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 6,
            "tb3_name": "BS01G01RE01",
            "tb3_full_name": "給線空軸準備區(1200)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 7,
            "tb3_name": "BS01G02RE01",
            "tb3_full_name": "給線空軸準備區(1200)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 8,
            "tb3_name": "BS01G03RE01",
            "tb3_full_name": "給線空軸準備區(1200)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 9,
            "tb3_name": "BS01G04RE01",
            "tb3_full_name": "給線空軸準備區(1200)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 10,
            "tb3_name": "BS01G05RE01",
            "tb3_full_name": "給線空軸準備區(1200)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 11,
            "tb3_name": "BS01G01",
            "tb3_full_name": "給線機(1300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 12,
            "tb3_name": "BS01G02",
            "tb3_full_name": "給線機(1300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 13,
            "tb3_name": "BS01G03",
            "tb3_full_name": "給線機(1300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 14,
            "tb3_name": "BS01G04",
            "tb3_full_name": "給線機(1300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 15,
            "tb3_name": "BS01G05",
            "tb3_full_name": "給線機(1300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 16,
            "tb3_name": "BS01T01",
            "tb3_full_name": "捲取機(2300)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 17,
            "tb3_name": "BS01T01RF01",
            "tb3_full_name": "捲取滿軸準備區(2100)",
            "tb3_status": null
        },
        {
            "hmi_id": 1,
            "hmi_name": "HMI001",
            "hmi_status": null,
            "tb2_id": 1,
            "tb2_machine_number": "BS01",
            "tb2_name": "1+4集合機",
            "tb2_status": 0,
            "tb3_id": 18,
            "tb3_name": "BS01T01RE01",
            "tb3_full_name": "捲取空軸準備區(2200)",
            "tb3_status": null
        }
    ]
}

```


參考 [[機臺設定-API]] 說明: 

tb1.id = hmi_config.id, tb2.id = machines.id, tb3.name = virtual_devices.name 對應資料表欄位名稱.