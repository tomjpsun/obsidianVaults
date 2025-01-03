
安裝 NoMachine 後連線，server 端都可以看到 client 端移動滑鼠的效果，就是看不到螢幕。
問題在此：
https://kb.nomachine.com/AR04R01083

採用 `option 1` 解決了：

> [!NOTE] 修改 /usr/NX/etc/node.cfg 設定爲：
> WaylandModes "compositor,drm"

修改後，當然要重啓服務：`sudo systemctl restart nxserver`

如果遇到停止在灰色畫面，可能是 keyring 的問題，此時需要 `disable Gnome Keyring`，將它設定爲空密碼即可，過程節錄如下：


> [!NOTE] 設定 Gnome Keyring 的參考網站：
> https://www.baeldung.com/linux/unlock-keyring-fix
>

==節錄==

## How to Disable Keyring?[](https://www.baeldung.com/linux/unlock-keyring-fix#how-to-disable-keyring)

The above-provided solutions will resolve issues with incorrect or forgotten keyring passwords. However, the _Unlock Keyring_ message will continue to prompt whenever we open any application that requires the keyring feature. **If we wish, we can disable the keyring feature by removing its password**.

We must be aware that disabling this feature can raise security concerns as anyone can now perform some administrative tasks and open applications storing personal data.

To disable a keyring, we open the _Passwords and Keys_ application and right-click on our desired (_Login_) keyring directory.

Now, from the drop-down list, we select the _Change Password_ option:

![select change password option](https://www.baeldung.com/wp-content/uploads/sites/2/2024/02/disable-password-1.png)

Next, enter our keyring password and press the _Continu_e button to proceed:

![enter old password](https://www.baeldung.com/wp-content/uploads/sites/2/2024/02/disable-password-1-1.png)

Then, simply leave the new password blank and press the _Continue_ button. **By leaving the password blank, we disable the keyring feature**:

![leave the password blank](https://www.baeldung.com/wp-content/uploads/sites/2/2024/02/disable-password-2.png)

Afterward, we’ll encounter a prompt message displaying that now our passwords will be stored unencrypted (plain text). Here, we click on the _Continue_ button:

![store passwords unencrypted](https://www.baeldung.com/wp-content/uploads/sites/2/2024/02/disable-password-3.png)

Now, we won’t get the _Unlock Keyring_ message. To verify this, let’s lock our keyring and try to access Google Chrome:

![verification of disabled keyring password](https://www.baeldung.com/wp-content/uploads/sites/2/2024/02/disabled-password.png)

Finally, the keyring feature is disabled.

==finished==
