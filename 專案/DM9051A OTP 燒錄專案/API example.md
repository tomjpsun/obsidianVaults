int writeRegisters(int nSlave, int nRegister, QByteArray aryData);	
nRegister 是 1-based .

例如要寫 VendorID (offset = 8)
writeRegisters(2, 8+1+BASE_MEMORY_ADDRESS , byteDaata);

**參考 spec**

| Name | Word | Offset/Byte | Description |
| ---- | ---- | ---- | ---- |
| MAC ID SET 1 | 0 | 0~5 | 6 Byte Ethernet Address Set 1 |
| Auto Load Control | 3 | 6~7 | Bit 1:0 = 01: Update vendor ID and product ID from WORD4 and 5. <br>Bit 3:2 = 01: Accept setting of WORD6 [15,4:3] <br>Bit 5:4 = 01: Reserved, set to 00 in application <br>Bit 7:6 = 01: Accept setting of WORD7 [3:0] <br>Bit 9:8 = 01: Accept setting of WORD8[3:0] <br>Bit 11:10 = 01: Accept setting of WORD7 [7] <br>Bit 13:12 = 01: Accept setting of WORD7 [9:8] Bit 15:14 = 01: Accept setting of WORD7 [15:12] |
| Vendor ID | 4 | 8~9 | vendor ID (Default: 0A46H) |
| Product ID | 5 | 10~11 | product ID (Default: 9051H) |
| Pin Control / Control 1 | 6 | 12~13 | When word 3 bit [3:2]=01, these bits can control the INT pins polarity. <br>Bit 2:0: Reserved; set to 0 in application <br>Bit 3: INT pin is active low when set (default: active high) <br>Bit 4: INT pin is open-collected (default: push-pull output) <br>Bit 14:5: Reserved; set to 0 in application <br>Bit 15: Enable 802.3az, to MAC register 3FH bit [7] |
| Wake-up Mode Control | 7 | 17 | When byte 22 bit[5:4]=01, these bits can be controlled.<br>Bit 0: The WOL pin is active low when set (default: active high) <br>Bit 1: The WOL pin is in pulse mode when set (default: push-pull mode) <br>Bit 2: Magic wakeup event is enabled when set. (default: disable) <br>Bit 3: Link change wakeup event is enabled when set (default disable) <br>Bit 6:4: Reserved; set to 0 in application <br>Bit 7:0 = LED mode 0, 1=LED mode 1 (default: mode 0) <br><br> |
| Control 3 | 8 | 16~17 | Bit 0: Reserved; set to 1 in application. <br>Bit 1: Eliminate SPI_CSB high spike control <br>This bit will be load into MAC register 38H bit 2 <br>Bit 3:2: SPI_MISO driving capability <br>This bit will be load into MAC register 38H bit [6:5] |