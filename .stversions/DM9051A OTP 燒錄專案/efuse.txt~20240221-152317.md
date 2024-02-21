Name Byte Description
MAC ID SET 1 O~5__|6 Byte Ethernet Address Set 1
MAC ID SET 2 6~11__|6 Byte Ethernet Address Set 2
MAC ID SET 3 12~17 |6 Byte Ethernet Address Set 3
Valid if byte 23[5:4] and [3:2] and [1:0] are all not equal to "01"
Vendor ID_LO. 12  vendor ID low byte (Default: 46H)
Vendor ID_HI 13  vendor ID high byte (Default: OAH)
Product ID_LO 14 [product ID low byte (Default: 51H)
Product ID_HI 15 [product ID high byte (Default: 90H)
SPI Pin control 16 |When byte 22 bit [3:2]=01, these bits can be controlled.
Bit 4~0: Reserved, set to 0 in application
Bit 5: Eliminate SPI_CSB high spike control
This bit will be load into register 38H bit 2
Bit 7~6: SPI_MISO driving capability
This bit will be load into register 38H bit [6:5]
Wake-up mode 17  When byte 22 bit [5:4]=01, these bits can be controlled.
control Bit 0: The WOL pin is active low when set (default: active high)
Bit 1: The WOL pin is in pulse mode when set (default: level mode)
Bit 2: Magic wakeup event is enabled when set. (default: disable)
Bit 3: Link change wakeup event is enabled when set (default disable)
Bit 6~4: Reserved; set to 0 in application
Bit 7 = LED mode 0, 1=LED mode 1 (default: mode 0)
TRIM_RES 18 [Trim Value of internal 500hm Resistor
Bit 7~6 = 01: Accept setting of byte 18 [5:0] as trim data PHY 500hm
CODE1 19 |Encryption/decryption code 1
Valid if not equal to OxFF
CODE2 20 _ |Encryption/decryption code 2
Valid if not equal to OxFF and CODE‘ is not valid.
CODE3 a1 lEncryption/decryption code 3
Valid if not equal to OxFF and CODE1/2 are not valid.
Auto Load Control 22 |When EEPROM is not exist,
Bit 1:0 = 01: Update vendor ID and product ID from byte 12~15.
Bit 3:2 = 01: Accept setting of byte 16 [5:0]
Bit 5:4 = 01: Accept setting of byte 17 [:
Bit 7:6 = 01: Accept setting of byte 23 [2:0]
When byte 22 bit 7:6=01, these bits can be controlled.
Bit 0: 1 = Internal PHY is enabled after power-on (default: disable)
Bit 1: PHY Fiber Mode Control; 1= Fiber mode, 0: TP mode
Bit 2: PHY enable 802.3az, to register 3FH bit 7
PHY/IRQ Control 23 ‘|Bit 3: IRQ pin is active low when set (default: active high)

Bit 4: IRQ pin is open-collected (default: force output)
Bit 5: CSB pin is active low when set (in uP mode, default set)
Bit 6: ORB pin is active low when set (in uP mode, default set)

Bit 7: IOWB pin is active low when set (in uP mode, default set)

