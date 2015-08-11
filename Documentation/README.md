## Instalation

* Download latest version of minbian:

```Shell
wget http://sourceforge.net/projects/minibian/files/latest/download
```

* Extract archive:

```Shell
tar -zxvf download
```

* Find SD card device: 
```Shell
mount
```
or
```Shell
dmesg
```

* Download image to sd card (remember to change device **/dev/sdd** to correct one):

```Shell
sudo dd if=2015-02-18-wheezy-minibian.img of=/dev/sdd bs=1M
```

* Flash the buffer content to card to make sure everything was written
```Shell
dmesg
```
 

