1 設定環境變數，search bar search *env*
![[Pasted image 20240521162040.png]]

2 進入*編輯我的帳戶的環境變數*，選 Path 編輯之，完成後如下圖：
![[Pasted image 20240521163102.png]]

3 新增一個是
{Qt Root}\\Tools\\QtInstallerFramework\\{Installer Version}\\bin
爲了可以執行 *binarycreator*， 這裏 Installer Version 是 4.8，Qt Root 是 C:\\Users\\Qt
新增另一個是 {Qt Root}\\Qt\\{Qt Version}，爲了可以執行 *windeployqt* ，這裏 Qt Version 是 6.7.0

*重開機使得環境變數生效*

		4 進入 Project， 用 Qt Creator build  release of Project， 程式碼應該會在 {Project}\\build\\Desktop_Qt_{Qt Version}\_64_bit-Release\\release，例如:

```
C:\Users\Qt_Projects\{App}\build\Desktop_Qt_6_7_0_64_bit-Release\release\{ App}{.exe}?
```

5 在 Project 目錄下做出 新目錄：

{Project}
├── config <- **做出新目錄**
│   ├── config.xml
├── packages <- **做出新目錄**
│   └── com.davicom.ptp4l  <- **自己命名**
│       ├── data <- **做出新目錄**
│       │   └── Ptp4l  <- **從第 4 步 copy 過來**
│       └── meta <- **做出新目錄**
│           └── package.xml
├── output  <- **做出新目錄**, 將來產生的安裝檔放在裏面

config.xml 長這樣：
``` xml
<Installer>
    <Name>Ptp4l</Name>
    <Version>1.0.0</Version>
    <Title>Ptp4l Installer</Title>
    <Publisher>DAVICOM SW Team</Publisher>
    <StartMenuDir>Ptp4l</StartMenuDir>
    <TargetDir>@HomeDir@/InstallationDirectory</TargetDir>
</Installer>
```
 其中 InstallationDirectory 改爲我們想要的路徑名稱，例如 "PTP4L"
 package.xml 長這樣：
```xml
<Package>
    <DisplayName>Ptp4l</DisplayName>
    <Description>This is the Ptp4l application.</Description>
    <Version>1.0.0</Version>
    <ReleaseDate>2024-05-21</ReleaseDate>
    <Default>true</Default>
</Package>
```

 裏面需要注意的是  *TargetDir* , 用 Installer 預設的變數，代表 user 的 Home dir 與 預設的安裝目錄。

6 用 console 進到 第 5 步的  data 目錄，然後 將第 4 步的 app copy 過來。

7 data 目錄下，現在只有一個 app 程式:

``` shell
PTP@DESKTOP-3HHHQQF MINGW64 ~/Documents/QT_exercises/Ptp4l/packages/com.davicom.ptp4l/data (main)
$ ls
Ptp4l.exe*
```

下指令 `windeployqt` 將 dependency libraries 拉進來:
``` bash
$ windeployqt Ptp4l.exe
```

看到 app 連同 dll 都放進來了：
``` shell
PTP@DESKTOP-3HHHQQF MINGW64 ~/Documents/QT_exercises/Ptp4l/packages/com.davicom.ptp4l/data (main)
$ ls
D3Dcompiler_47.dll*    Qt6Pdf.dll*          libwinpthread-1.dll*
Ptp4l.exe*             Qt6Svg.dll*          networkinformation/
Qt6Charts.dll*         Qt6Widgets.dll*      opengl32sw.dll*
Qt6Core.dll*           generic/             platforms/
Qt6Gui.dll*            iconengines/         styles/
Qt6Network.dll*        imageformats/        tls/
Qt6OpenGL.dll*         libgcc_s_seh-1.dll*  translations/
Qt6OpenGLWidgets.dll*  libstdc++-6.dll*
```


8 回到 Project 目錄，下指令

``` bash
binarycreator -c config/config.xml -p packages output/installer.exe
```

就會在 output 目錄下產生我們的安裝檔。

_The end_

