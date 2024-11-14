**RENESAS****白皮書**

**Linux** **核心對 IEEE 1588** **硬體時間戳的支持**

**原文頁面 1**

Michael Rupert, Principle System Design Engineer, Data Center Business Division

Michael Rupert, 主要系統設計工程師, 數據中心業務部

# 摘要(Abstract)

本白皮書討論了 Linux 核心對 IEEE 1588 硬體時間戳的支持，包括精確時間協議 (PTP) 硬體時鐘 (PHC) 基礎設施，以及 SO_TIMESTAMPING 套接字選項。PHC 基礎設施和 SO_TIMESTAMPING 套接字選項為用戶空間應用程序和 Linux 核心設備驅動程序提供了標準化的 API。這些核心功能提高了設備驅動程序的可用性並簡化了系統集成。

# 介紹(Introduction)

IEEE 1588 定義了一種協議，即精確時間協議 (PTP)，該協議能夠在分組交換網絡 (PSN) 上實現精確的同步。PTP 事件消息的硬體時間戳是實現 PTP 從屬時鐘納秒同步精度的關鍵。

A typical PTP slave clock with hardware time stamping can be divided into four components:
1. User-space software that implements an IEEE 1588 protocol stack and a PTP clock servo
2. Hardware time stamp unit (TSU) integrated with a MAC or PHY
3. PTP hardware clock (PHC) that provides the timing reference for the hardware TSU and that is controlled by the PTP clock servo
4. Linux kernel(Linux 核心)

Linux 核心實現了對 PTP 事件消息的硬體時間戳的內建支持。該支持由 PHC 基礎設施和 SO_TIMESTAMPING 套接字選項組成。這些核心功能為 PHC 和 TSU 功能提供了標準化的用戶空間 API，並為 PHC 和 TSU 設備驅動程序提供了標準化的接口。對於系統集成商而言，最終結果是提高了設備驅動程序的可用性並簡化了系統集成，從而降低了開發成本並縮短了上市時間。

為了實現 Linux 中 PTP 支持的好處，PHC 設備驅動程序必須支持 PHC 基礎設施；而 TSU 驅動程序必須支持 SO_TIMESTAMPING 套接字選項。

Renesas 提供了一個支持 Linux PHC 基礎設施的 ClockMatrix 家族的 Linux 核心驅動程序。任何使用 Linux PHC API 的 IEEE 1588 軟體包都可以使用此 API 控制 ClockMatrix 設備。例如，來自網絡時間基金會的 Linux PTP 設計用於使用 Linux PHC API。ClockMatrix Linux 核心驅動程序作為主線 Linux 的一部分提供。

Renesas PTP 時鐘管理器軟體包括一個具有封包延遲變化 (PDV) 濾波器的 PTP 時鐘伺服和其他功能，滿足全球 ITU-T 對電信應用的同步建議。PTP 時鐘管理器可以與任何 IEEE 1588 協議棧（例如，Linux PTP）一起使用，並且與 Linux PHC API 兼容。PTP 時鐘管理器可根據 Renesas 的許可使用。


**原文頁面 3**

# 定義(Definitions)

+ Clock
	+ IEEE 1588 定義時鐘如下：一種可以提供自定義紀元以來時間流逝的測量的設備。

+ Clock Signal
	+ IEEE 1588 定義時鐘信號如下：一種具有周期性事件的物理信號。周期性事件標記了時間計數器增量的顯著瞬間。時鐘信號的特徵是其頻率和相位。
+ IEEE 1588
	+ Precision Time Protocol – IEEE 1588 定義了一種協議，精確時間協議（PTP），使得在分組交換網絡（PSN）上實現準確的同步。PTP 擁有廣泛的應用，包括電信、工業、企業等。PTP 可用於以納秒精度同步 PTP 從屬時鐘。

註解1：IEEE Std 1588-2019, Standard for a Precision Clock Synchronization Protocol for Networked Measurement and Control Systems.

# PTP 從屬時鐘的基本同步(Basic Synchronization for a PTP Slave Clock)

圖 1 顯示了 PTP 主時鐘和 PTP 從屬時鐘之間的基本 PTP 事件消息交換；還顯示了一個作為伺服迴路實現的 PTP 從屬時鐘，配有時鐘伺服控制器（時鐘伺服）。在理想條件下，時鐘伺服可以使用 EQ1 計算其本地時間戳單元（TSU）與主時鐘的 TSU 之間的時間偏移。時間偏移由時鐘伺服進行數字補償；之後，PTP 從屬時鐘信號的頻率會定期調整，以保持時間偏移接近零。因此，PTP 從屬時鐘與 PTP 主時鐘同步並鎖定。


![[Linux Kernel Support for IEEE 1588 Hardware Timestamping01.png]]
圖 1. 基本精確時間協議事件消息交換

圖一內文解說：
TSU = 時間戳單元
TSU 通過計數已知名義頻率的時鐘信號的週期來測量時間的流逝。
t 1 , t 2 , t 3 , t 4 由 TSU 測量並為 PTP 從屬時鐘所知；它們用於計算 PTP 從屬時鐘的時間偏移。


**原文頁面 4**

# 硬體時間戳記的 PTP 事件消息(Hardware Timestamping of PTP Event Messages)

硬體時間戳記的 PTP 事件消息是實現 PTP 從屬時鐘納秒同步精度的關鍵。TSU 應該在信號路徑中盡可能接近物理層。硬體 TSU 通常實現在網路介面設備中，例如 MAC 或 PHY。與軟體時間戳記相比，硬體時間戳記將 PTP 事件消息的到達和離開時間的不確定性從毫秒減少到納秒；因此，硬體時間戳記提高了 PTP 從屬時鐘的精度。

硬體 TSU 包含一個日常時間 (TOD) 累加器，通過計算 PTP 硬體時鐘 (PHC) 的參考時鐘信號的週期來測量時間的流逝。PHC 由 PTP 時鐘伺服控制，該伺服對 PHC 發出修正（時鐘操作）。IEEE 1588 協議堆棧從硬體 TSU 獲取 PTP 時間戳信息並將其提供給時鐘伺服。這在圖 2 中說明。


![[Linux Kernel Support for IEEE 1588 Hardware Timestamping02.png]]
**圖 2.** **使用硬體時間戳記的典型 PTP** **從屬時鐘實現**

邏輯 PHC 和 TSU 功能可以在單一的矽設備中實現，也可以在不同的設備中實現。獨立的計時設備作為生成和管理在多個設備中共享的時鐘信號的中央資源具有優勢。因此，PHC 功能通常在專用的計時設備中實現。

圖 2 顯示了在不同設備中實現的 TSU 和 PHC 功能。

對於 PTP 系統集成商來說，一個長期挑戰是獲取和維護 PHC 和硬體 TSU 的驅動程式。對於矽設備製造商來說，創建驅動程式以涵蓋行業中可用的多個 PTP 協議堆棧和時鐘伺服軟體包是低效的。因此，設備製造商通常提供示例驅動程式代碼，並將驅動程式開發留給系統集成商。

Linux 核心實現了對 PTP 事件消息的硬體時間戳記的內建支持。該支持由 PHC 基礎設施和 SO_TIMESTAMPING 套接字選項組成（見圖 2）。這些核心


> [!NOTE] 
> 2. 優勢包括：以亞納秒解析度調整時鐘偏差、測量時鐘延遲、生成低相位噪聲時鐘、生成具有多種頻率的時鐘、生成符合 ITU-T 的同步以太網時鐘，以及管理冗餘時鐘源等。

>[!Note] Figure 2 翻譯
> MAC 封包交換網路 PTP 封包 PHC
> 
> 本地振盪器 PHC 基礎設施 SO_TIMESTAMPING 套接字選項 IEEE 1588 協議堆棧 核心驅動 PTP 時鐘 時鐘伺服 PTP 從屬時鐘 TSU = 時間戳單元 PHC = PTP 硬體時鐘 TOD = 日常時間 PPS = 每秒脈衝 TOD PHC PTP 主時鐘 TOD M PTP 封包 t1, t2, t3, t4 伺服迴路 PTP 時間戳
> 
> 時鐘操作 核心驅動 硬體 TSU
> 
> TOD TSU GPL-2 軟體 Linux 核心 用戶空間 TOD PPS 時鐘信號 標準介面 標準介面。

**原文頁面 5**

設施提供標準化的用戶空間 API 用於 PHC 和 TSU 功能，並且它們提供標準化的接口給 PHC 和 TSU 設備驅動程序。開發者可以為標準的 Linux API 編寫他們的用戶空間 PTP 應用程序，而無需詳細了解最終系統中將使用的 PHC 或 TSU 設備。同樣，矽設備製造商可以使用標準的 Linux 內核 API 編寫和驗證 Linux 內核設備驅動程序，而無需詳細了解將使用它們的用戶空間應用程序。對於系統集成商來說，最終結果是設備驅動程序的可用性提高，系統集成簡化，從而降低開發成本並縮短上市時間。

Linux 內核對 PTP 的支持有助於確保與新內核版本的向前兼容性。Linux 內核開發者努力在內核演變的過程中保持穩定的用戶空間 API，並且作為主線 Linux 一部分的內核設備驅動程序在每次版本發布中都會得到維護。此外，Linux 社區和矽設備製造商可以對驅動程序進行改進，並且每個使用它們的系統都可以輕鬆採納這些改進。

為了實現 Linux 中 PTP 支持的好處，PHC 設備驅動程序必須支持 PHC 基礎設施；而 TSU 驅動程序必須支持 SO_TIMESTAMPING 套接字選項。基於以下描述的原因，TSU 驅動程序支持其 TOD 累加器的 PHC 基礎設施也是有用的。SO_TIMESTAMPING 套接字選項和 PHC 基礎設施在 Linux 內核版本 3.0 及更高版本中同時可用。

# 同步 TSU 與獨立 PHC(Synchronizing a TSU with a Separate PHC)

當 PHC 和 TSU 被單獨實現時，系統集成商必須同步 TOD TSU 與 TOD PHC。這可以通過使用相同的 PHC 時鐘和每秒脈衝 (PPS) 信號來計時 TOD TSU，這些信號也用於計時 TOD PHC。在系統啟動時，TOD TSU 和 TOD PHC 之間會有一個靜態時間偏移，必須消除；有一個名為 ts2phc 的開源用戶空間應用程序可用於此目的。ts2phc 實用程序將 TOD TSU 控制為 PHC；前提是 TSU 驅動程序必須支持 PHC 基礎設施。在對兩個 TOD 對齊後，對 PHC 所做的任何頻率調整將由 TSU 追蹤。

# Renesas PTP 同步解決方案(Renesas PTP Synchronization Solutions)

Renesas ClockMatrix 系列定時設備包括幾個設計用作 PHC 的產品（例如，8A34001/2/3/4/5 是電信 PHC 應用的優秀設備）。Renesas 為 ClockMatrix 系列創建了一個支持 Linux PHC 基礎設施的 Linux 內核驅動程序。ClockMatrix 驅動程序作為主線 Linux 的一部分可用，並且與 Linux 內核版本 3.0 及更高版本兼容。

## 僅使用開源軟件的 PTP 同步解決方案(PTP Synchronization Solution Using Open Source Software Only)

ClockMatrix Linux 內核驅動程序與 Linux PTP 軟件套件版本 3.0 及更高版本兼容，包括其 IEEE 1588-2019 協議棧，以及包含的 PTP 時鐘伺服和 ts2phc 實用程序。Linux PTP 是一個開源 (GPL-2) 軟件套件，來自網絡時間基金會。

當與 ClockMatrix 設備一起使用時，Linux PTP 將繞過其 PTP 時鐘伺服的軟件低通濾波器功能，而是使用 ClockMatrix 設備實現的寫入相位模式和硬件低通濾波器。這種伺服實現適合不需要處理高水平的數據包延遲變化 (Packet Delay Variation) PDV 的應用。

> [!NOTE]
> 3. ts2phc 實用程序包含在來自網絡時間基金會的 Linux PTP 軟件套件中。
> 
> 4. 有關更多信息，請參見 Renesas 白皮書，標題為 限制由數據包延遲變化引起的 IEEE 1588 從屬時鐘漂移。

**原文頁面 6**

圖 3 顯示了一個僅使用開源軟體的PTP從時鐘設計，適用於不需要處理高水平PDV的應用。


![[Linux Kernel Support for IEEE 1588 Hardware Timestamping03.png]]
圖 3. Renesas PTP從時鐘實現僅使用開源軟體

### 使用開源和Renesas軟體的PTP同步解決方案(PTP Synchronization Solution Using Open Source and Renesas Software)

圖 3中顯示的設計適用於許多PTP應用。然而，對於需要容忍PDV和全球遵循ITU-T同步建議的電信應用，則需要更複雜的PTP時鐘伺服。

及其他功能，符合全球ITU-T同步建議的電信應用。PTP時鐘管理器與Linux PTP版本3.0及更高版本兼容，特別是IEEE 1588-2019協議棧；它也與Linux內核版本3.0及更高版本兼容。

Renesas PTP時鐘管理器使用其自己的替代方案ts2phc將TOD TSU與TOD PHC同步。在將TOD TSU與TOD PHC同步時，PTP時鐘管理器將TOD TSU控制為PHC；前提是TSU驅動程式必須支持PHC基礎設施。

圖 4 顯示了一個使用Renesas PTP時鐘管理器軟體的PTP從時鐘設計。這個時鐘設計適用於需要管理高水平PDV和/或需要遵循全球ITU-T同步建議的電信應用。

> [!NOTE] Figure 3 & 4 翻譯
> MAC封包交換網路 PTP封包
> 
> Renesas ClockMatrix 本地振盪器 PHC基礎設施 SO_TIMESTAMPING 套接字選項
> 
> Linux PTP v3+ IEEE 1588 協議棧 內核驅動
> 
> Linux PTP3+ PTP時鐘 時鐘伺服 PTP從時鐘 TOD PHC PTP主時鐘 TOD M PTP封包 t 1 , t 2 , t 3 , t 4 伺服迴路 內核驅動 硬體 TSU TOD TSU Renesas SW Renesas HW 非Renesas SW 非Renesas HW GPL-2 SW TOD PPS時鐘信號 Linux PTP套件 TSU = 時間戳單元 PHC = PTP硬體時鐘 TOD = 當前時間 PPS = 每秒脈衝
> 
> Linux內核用戶空間 PTP時間戳 時鐘操作。

**原文頁面 7**


![[Linux Kernel Support for IEEE 1588 Hardware Timestamping04.png]]
圖 4. Renesas PTP 從屬時鐘實現使用 PTP 時鐘管理器軟體

# 結論(Conclusion)

PTP 事件消息的硬體時間戳記是實現 PTP 從屬時鐘納秒同步精度的關鍵。硬體時間戳記是通過由 PHC 生成的時鐘信號來定時的硬體 TSU 實現的。硬體 TSU 通常與 PHY 和 MAC 集成，以便靠近物理層。

邏輯 PHC 和 TSU 功能可以在單一的矽晶片設備中實現，也可以在不同的設備中實現。獨立的計時設備作為生成和管理在多個設備中共享的時鐘信號的中央資源具有優勢。因此，PHC 功能通常在專用的計時設備中實現。

Linux 核心實現了對 PTP 事件消息的硬體時間戳記的內建支援。該支援由 PHC 基礎架構和 SO_TIMESTAMPING 套接字選項組成。這些核心設施為 PHC 和 TSU 功能提供了標準化的用戶空間 API，並為 PHC 和 TSU 設備驅動程序提供了標準化的介面。對於系統集成商而言，最終結果是設備驅動程序的可用性提高，系統集成簡化，從而降低開發成本並縮短上市時間。

為了實現 Linux 中 PTP 支援的好處，PHC 設備驅動程序必須支援 PHC 基礎架構；而 TSU 驅動程序必須支援 SO_TIMESTAMPING 套接字選項。對於 TSU 驅動程序來說，支援其 TOD 累加器的 PHC 基礎架構也是有用的。

Renesas ClockMatrix 系列計時設備是實現硬體時間戳記應用的優秀選擇。Renesas 提供了支援 Linux PHC 基礎架構的 ClockMatrix 系列的 Linux 核心驅動程序。

Renesas PTP 時鐘管理器軟體實現了一個具有 PDV 濾波器和其他功能的 PTP 時鐘伺服，符合全球 ITU-T 同步建議，用於電信應用。PTP 時鐘管理器與 Linux PHC 基礎架構相容。

標準化的 PHC 基礎架構和 SO_TIMESTAMPING 套接字選項 API 使 PTP 功能的模組化成為可能。例如，Renesas PTP 時鐘管理器可以替代 Linux PTP 伺服，用於需要 PDV 濾波的應用。同樣，ClockMatrix 設備中的硬體低通濾波器可以替代 Linux PTP 伺服中的軟體低通濾波器。

UDS.

# Revision History

|   |   |   |
|---|---|---|
|Revision|Date|Description|
|1.0|Jan.29.21|Initial release.|

# IMPORTANT NOTICE AND DISCLAIMER

RENESAS ELECTRONICS CORPORATION AND ITS SUBSIDIARIES (“RENESAS”) PROVIDES TECHNICAL SPECIFICATIONS AND RELIABILITY DATA (INCLUDING DATASHEETS), DESIGN RESOURCES (INCLUDING REFERENCE DESIGNS), APPLICATION OR OTHER DESIGN ADVICE, WEB TOOLS, SAFETY INFORMATION, AND OTHER RESOURCES “AS IS” AND WITH ALL FAULTS, AND DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT OF THIRD-PARTY INTELLECTUAL PROPERTY RIGHTS. These resources are intended for developers who are designing with Renesas products. You are solely responsible for (1) selecting the appropriate products for your application, (2) designing, validating, and testing your application, and (3) ensuring your application meets applicable standards, and any other safety, security, or other requirements. These resources are subject to change without notice. Renesas grants you permission to use these resources only to develop an application that uses Renesas products. Other reproduction or use of these resources is strictly prohibited. No license is granted to any other Renesas intellectual property or to any third-party intellectual property. Renesas disclaims responsibility for, and you will fully indemnify Renesas and its representatives against, any claims, damages, costs, losses, or liabilities arising from your use of these resources. Renesas' products are provided only subject to Renesas' Terms and Conditions of Sale or other applicable terms agreed to in writing. No use of any Renesas resources expands or otherwise alters any applicable warranties or warranty disclaimers for these products.

如需有關產品、技術、最新版本文件或您最近的銷售辦事處的更多資訊，請訪問 www.renesas.com/contact-us/.

Renesas 和 Renesas 標誌是 Renesas Electronics Corporation 的商標。所有商標和註冊商標均為其各自所有者的財產。

RENESAS ELECTRONICS CORPORATION 及其子公司（“RENESAS”）提供技術規格和可靠性數據（包括數據表）、設計資源（包括參考設計）、應用或其他設計建議、網絡工具、安全資訊及其他資源“按現狀”提供，並且存在所有缺陷，並且不承擔任何明示或暗示的保證，包括但不限於任何暗示的適銷性保證、特定用途的適用性或不侵犯第三方知識產權的保證。這些資源旨在供設計使用 Renesas 產品的開發人員使用。您對（1）選擇適合您應用的產品、（2）設計、驗證和測試您的應用，以及（3）確保您的應用符合適用標準及任何其他安全、保安或其他要求，負有唯一責任。這些資源可能會在不通知的情況下更改。Renesas 授予您使用這些資源的許可僅限於開發使用 Renesas 產品的應用。其他複製或使用這些資源是嚴格禁止的。對於任何其他 Renesas 知識產權或任何第三方知識產權，均不授予許可。Renesas 對於因您使用這些資源而產生的任何索賠、損害、成本、損失或責任不承擔責任，您將完全賠償 Renesas 及其代表。Renesas 的產品僅根據 Renesas 的銷售條款和條件或其他書面同意的適用條款提供。對於這些產品，任何 Renesas 資源的使用均不擴展或以其他方式改變任何適用的保證或保證免責聲明。（免責聲明 Rev.1.01 2024年1月）© 2024 Renesas Electronics Corporation。保留所有權利。



# 縮寫 (Abbreviations)

time stamp unit (TSU)

packet switched networks (PSN)

Precision Time Protocol (PTP)

|   |   |   |
|---|---|---|
|**Abbreviation**|**Definition**||
|MIB|Management Information Base||
|SNMP|Simple Network Management Protocol||
|SMI|Structure of Management Information||
|PTP|Precision Time Protocol||
|TSN|Time-Sensitive Networking||
|E2E|End-to-End||
|EUI|Extended Unique Identifier||
|GPS|Global Positioning System||
|IANA|Internet Assigned Numbers Authority||
|IP|Internet Protocol||
|NTP|Network Time Protocol (see [RFC5905])||
|P2P|Peer-to-Peer||
|PTP|Precision Time Protocol|精確時間協議|
|TAI|International Atomic Time||
|UDP|User Datagram Protocol||
|UTC|Coordinated Universal Time||

  原文網址：https://www.renesas.com/en/document/whp/linux-kernel-support-ieee-1588-hardware-timestamping?language=en


