---
tags:
  - mfc1910w
---
[Source](https://medium.com/@alexanderbelov/how-to-use-your-brother-printer-with-cups-on-raspberry-pi-5b712cc2b4e6)

Although Brother has its own drivers for CUPS, they’re precompiled for x86 (32-bit) architecture. Raspberry Pi is based on ARM arch, so manufactures’ drivers are note compatible with your Pi out of the box.

New to Raspberry Pi and CUPS Print Server?  
[Check out this article](https://medium.com/swlh/setup-a-print-server-using-raspberry-pi-cups-part-1-c8f3d48af047) to make your own local printer server to print wirelessly to your printer (including native support on iOS and Mac devices).

# Solution

Software enthusiast Peter De Wachter has written ==an alternate driver for Brother’s printers== − it’s called «brlaser».

This driver brings support for the following list of models:

- Brother DCP-1510
- Brother DCP-1602
- Brother DCP-7030
- Brother DCP-7040
- Brother DCP-7055
- Brother DCP-7055W
- Brother DCP-7060D
- Brother DCP-7065DN
- Brother DCP-7080
- Brother DCP-L2500D
- Brother DCP-L2540DW
- Brother HL-1110 series
- Brother HL-1200 series
- Brother HL-L2300D series
- Brother HL-L2320D series
- Brother HL-L2340D series
- Brother HL-L2360D series
- Brother MFC-1910W
- Brother MFC-7240
- Brother MFC-7360N
- Brother MFC-7365DN
- Brother MFC-7840W
- Brother MFC-L2710DW
- Lenovo M7605D

Source code is available at its [GitHub repo](https://github.com/pdewacht/brlaser).

**If you don’t see your model here, don’t be upset:** your model can be already supported by CUPS built-in drivers − in that case you don’t need to install any additional drivers.

Installation is quite simple − just paste this line to Terminal:

==sudo apt-get install printer-driver-brlaser==

Right after that restart your CUPS server with this command to reload list of installed drivers:

sudo service cups restart

Now login to CUPS (link is [http://localhost:631/](http://localhost:631/), if you didn’t change this).  
Click on «Administration», then press on «Add Printer» button.

When CUPS will ask you to choose the driver to communicate with your printer, choose «brlaser».

If you don’t see the exact model of your printer, pick the closest option based on your model’s number. For example, I have an 7060D model. Most close option is 7065 model − most probably that will work like a charm.

![](https://miro.medium.com/v2/resize:fit:933/1*nEGxxfVwb79wUg4uN-K93g.png)

> These keywords may help you find this article via search engines:  
> brother printer cups, cups brother printer is not printing, brother cups drivers, cups brother printers

