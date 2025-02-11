apt 不小心引入 i386 相關的套件，現在 aptitude 都看得到 -amd 與 -i386 套件，如何只看到 i386 ?

1. Show what foreign architectures are installed: `dpkg --print-foreign-architectures`
    - Might show: `i386`
2. Review `i386` packages on your system: `dpkg -l | grep i386`
3. Remove all `i386` packages: `apt-get purge ".*:i386"`
    - Note: The `purge` keyword (instead of `remove`) removes all configuration files associated with the packages you're uninstalling. (Thanks PCGuyIV!)
#aptitude #i386
[出處](https://superuser.com/questions/714391/how-do-i-remove-all-i386-architecture-packages-from-my-debian-installation)

