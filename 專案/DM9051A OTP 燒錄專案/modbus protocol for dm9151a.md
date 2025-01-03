
**Function 04 (04hex) Read Input Registers**

Read the binary contents of input registers in the slave.

**Request**The request message specifies the starting register and quantity of registers to be read.

Example of a request to read 0...1 (register 30001 to 30002) from slave device 1:

|   |   |   |
|---|---|---|
|**Field Name**|**RTU (hex)**|**ASCII Characters**|
|Header|None|: (Colon)|
|Slave Address|01|0 1|
|Function|04|0 4|
|Starting Address Hi|00|0 0|
|Starting Address Lo|00|0 0|
|Quantity of Registers Hi|00|0 0|
|Quantity of Registers Lo|02|0 2|
|Error Check Lo|71|LRC (F 9)|
|Error Check Hi|CB|None|
|Trailer|None|CR LF|
|Total Bytes|8|17|



**Response
**The register data in the response message are packed as two bytes per register, with the binary contents right justified within each byte. For each register the first byte contains the high-order bits, and the second contains the low-order bits.

Example of a response to the request:

|   |   |   |
|---|---|---|
|**Field Name**|**RTU (hex)**|**ASCII Characters**|
|Header|None|: (Colon)|
|Slave Address|01|0 1|
|Function|04|0 4|
|Byte Count|04|0 4|
|Data Hi|00|0 0|
|Data Lo|06|0 6|
|Data Hi|00|0 0|
|Data Lo|05|0 5|
|Error Check Lo|DB|LRC (E C)|
|Error Check Hi|86|None|
|Trailer|None|CR LF|
|Total Bytes|9|19|








**Function 16 (10hex) Write Multiple Registers**

Writes values into a sequence of holding registers

**Request**The request message specifies the register references to be written. Registers are addressed starting at zero-register 1 is addressed as 0.

The requested write values are specified in the request data field. Data is packed as two bytes per register.

Here is an example of a request to write two registers starting at 40002 to 00 0A and 01 02 hex, in slave device 17:

|   |   |   |
|---|---|---|
|**Field Name**|**RTU (hex)**|**ASCII Characters**|
|Header|None|: (Colon)|
|Slave Address|11|1 1|
|Function|10|1 0|
|Starting Address Hi|00|0 0|
|Starting Address Lo|01|0 1|
|Quantity of Registers Hi|00|0 0|
|Quantity of Registers Lo|02|0 2|
|Byte Count|04|0 4|
|Data Hi|00|0 0|
|Data Lo|0A|0 A|
|Data Hi|01|0 1|
|Data Lo|02|0 2|
|Error Check Lo|C6|LRC (C B)|
|Error Check Hi|F0|None|
|Trailer|None|CR LF|
|Total Bytes|13|23|



**Response
**The normal response returns the slave address, function code, starting address, and quantity of registers written. Here is an example of a response to the request shown above.

|   |   |   |
|---|---|---|
|**Field Name**|**RTU (hex)**|**ASCII Characters**|
|Header|None|: (Colon)|
|Slave Address|11|1 1|
|Function|10|1 0|
|Starting Address Hi|00|0 0|
|Starting Address Lo|01|0 1|
|Quantity of Registers Hi|00|0 0|
|Quantity of Registers Lo|02|0 2|
|Error Check Lo|12|LRC (D C)|
|Error Check Hi|98|None|
|Trailer|None|CR LF|
|Total Bytes|8|17|
