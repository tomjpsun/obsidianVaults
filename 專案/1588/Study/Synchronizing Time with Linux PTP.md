gPTP 是 IEEE 1588 定義的 profile, 它定義了一份簡單的 PTP 規範, 涵蓋 GM 角色, 採用 **BMCA** (Best Master Clock Algorithm)
決定出 pTPT domain 的 GM.

本篇會拆分三部分介紹:
- **ptp4l**: daemon that synchronizes the PTP Hardware Clock (PHC) from the NIC;
- **phc2sys**: daemon that synchronizes the PHC and the System clock;
- **pmc**: utility tool to configure ptp4l in run-time.

本篇是以 Intel I210 網卡進行測試的, 透過學習本教程，讀者將能夠使用 Linux PTP 同步時間.

https://tsn.readthedocs.io/timesync.html


\#