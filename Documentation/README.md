## Instalation

* Download latest version of minbian:

```Shell
wget http://dl.bananian.org/releases/bananian-latest.zip
```

* Extract archive:

```Shell
unzip bananian-latest.zip
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
sudo dd if=bananian-1504.img of=/dev/sdd bs=4M
```

* Flash the buffer content to card to make sure everything was written
```Shell
sync
```
 

