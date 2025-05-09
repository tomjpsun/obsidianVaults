Add two lines to /etc/default/grub

```
GRUB_SAVEDEFAULT=true
GRUB_DEFAULT=saved
```

Do the `sudo update-grub`, reboot. vala!