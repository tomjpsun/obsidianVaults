---
tags:
  - 行事曆
  - calendar
  - CalDAV
---
Apple Calendar 支援 CalDAV protocol, 以下是設定方法，摘錄自
https://ar.al/2018/08/05/using-icloud-calendars-on-gnu-linux/

# Linux 需要安裝 Evolution 收信軟體與 Gnome Concates
```bash
sudo apt install gnome-contacts
sudo apt install evolution
```
#  Set up an app-specific password on iCloud.

進入 App 專用密碼

![[attachments/Screenshot from 2023-10-17 09-34-17.png]]

按 + 新增一個
![[attachments/Screenshot from 2023-10-17 09-34-42.png]]

#### ！ 重要！ copy 這裏的密碼，離開後就看不到了！

![[attachments/Screenshot from 2023-10-17 09-35-15.png]]


# Set up your calendar(s) in Evolution.

接下來設定 Evolution,
1. 從左上角的 New，新增一個 Calendar,
2. Type 選擇 CalDAV，Name 自行取一個名字
3. URL 必填 https://caldav.icloud.com/
4. User 填 Apple ID
5. 按下 Find Calendars 後，在 Pop window 裏面填寫前面取得的 Apple 專用密碼
6. 等一分鐘，會與 Apple iCloud sync 行事曆，完成
]]![[attachments/Screenshot from 2023-10-17 09-32-39 1.png]]