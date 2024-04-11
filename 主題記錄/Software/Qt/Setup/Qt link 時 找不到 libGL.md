
在安裝到筆電的 Debian 12 後，build program 的時候，有這個錯誤:
link error: -lGL 

[Source: Stack Overflow](https://stackoverflow.com/questions/18406369/qt-cant-find-lgl-error)

解決方式：
```bash
sudo apt install libgl1-mesa-dev
```


引用 Stack Overflow 裏面的回答：

	@Cuadue: The GL in the error and in libgl1-... is for OpenGL, a fundamental dependency of Qt 5.x. This is a common error for devs when setting up for Qt development -- but once they install the OpenGL development lib they never see it again.


