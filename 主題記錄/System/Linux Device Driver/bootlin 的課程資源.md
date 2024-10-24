source: https://coldnew.github.io/3dbd10f2/

最近在 GitHub 上剛好看到 [bootlin](https://bootlin.com/) 這間專做 Embedded Linux 相關研發與顧問的公司所開源的 [教育訓練教材](https://github.com/bootlin/training-materials/) ，由於原始檔是用 latex 等工具撰寫，因此本文主要在講怎樣將其轉換成 PDF 檔案。

## 關於 bootlin

[bootlin](https://bootlin.com/company/about-us/) (以前名為 Free Electronic) 是位於法國的公司，主要做的是和 Embedded Linux 相關的研發，以及提供教育訓練課程給那些想接觸 Open Source 以及 Embedded Linux 的公司，也就是作為顧問公司來引導那些對 Embedded Linux 不熟或是在上面遇到問題無法解決的公司們。

當然，既然作為一個顧問公司就要拿點成果出來，也因此這間公司會定時在 blog 上寫他們對 Linux Kernel 或是其他 Open Source 的貢獻，來證明自己不是間只會出一張嘴的顧問，以下列舉部份資訊:

- [Linux 5.3 released, Bootlin contributions inside](https://bootlin.com/blog/linux-5-3-released-bootlin-contributions-inside/)
- [Improvements to Buildroot maintenance tooling](https://bootlin.com/blog/improvements-to-buildroot-maintenance-tooling/)
- [Bootlin at the Embedded Linux Conference Europe 2019](https://bootlin.com/blog/bootlin-at-the-embedded-linux-conference-europe-2019/)

而他們所開授的教育訓練課程，是開源在 [GitHub](https://github.com/bootlin/training-materials/) 的，因此你也可以自行透過該 repo 來取得最新的 [bootlin](https://bootlin.com/company/about-us/) 的教育訓練課程的投影片。

## 下載與安裝套件

如同前面所說，投影片位於 [bootlin/training-materials](https://github.com/bootlin/training-materials/) ，因此先透過 git 來下載

git clone https://github.com/bootlin/training-materials/

接下來，你的電腦需要裝一些套件才能順利編譯，如果是 Ubuntu 的話，大概要裝以下套件

sudo apt install --no-install-recommends git-core inkscape \
       texlive-latex-base texlive-latex-extra texlive-font-utils dia \
       python-pygments texlive-fonts-recommended texlive-fonts-extra make \
       texlive-xetex texlive-extra-utils fonts-inconsolata fonts-liberation \
       ttf-ubuntu-font-family xfonts-scalable latex-beamer lmodern

就這樣，我們可以開始嘗試將 latex 編譯成 pdf 檔囉~

## 編譯

編譯的方式很簡單，就進入到 `training-materials` 資料夾然後輸入 `make` 就會看到有哪些課程的 PDF 檔案可以編譯了

可以編譯了

```
make
Available targets:

 full-autotools-labs.pdf            Complete labs for the 'autotools' course
 full-boot-time-labs.pdf            Complete labs for the 'boot-time' course
 full-buildroot-labs.pdf            Complete labs for the 'buildroot' course
 full-embedded-linux-labs.pdf       Complete labs for the 'embedded-linux' course
 full-embedded-linux-4d-labs.pdf    Complete labs for the 'embedded-linux-4d' course
 full-graphics-labs.pdf             Complete labs for the 'graphics' course
 full-linux-kernel-labs.pdf         Complete labs for the 'linux-kernel' course
 full-yocto-labs.pdf                Complete labs for the 'yocto' course

skip ...

 <some-chapter>-slides.pdf          Slides for a particular chapter in slides/
 <some-chapter>-labs.pdf            Labs for a particular chapter in labs/

 list-courses                       List all courses
```

比如說我要編譯 `full-embedded-linux-labs.pdf` 這份檔案，就這樣

```
make full-embedded-linux-labs.pdf
make[1]: Entering directory '/home/coldnew/training-materials'
/bin/echo "\def \sessionurl {}" > /home/coldnew/training-materials/out/vars
/bin/echo "\def \training {embedded-linux}" >> /home/coldnew/training-materials/out/vars
/bin/echo "\def \trainer {}" >> /home/coldnew/training-materials/out/vars
DIA            flash-map.dia       ->       flash-map.eps
....
cat out/full-embedded-linux-labs.pdf > full-embedded-linux-labs.pdf
make[1]: Leaving directory '/home/coldnew/training-materials
```

然後就可以在當前目錄找到 `full-embedded-linux-labs.pdf` 這份檔案啦。

## 後記

其實編譯好的投影片在 [bootlin](https://bootlin.com/company/about-us/) 的網站上面可以找到，不過還是自己編譯比較快，也可以快速的 update 一些資訊。