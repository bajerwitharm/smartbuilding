# Instalation

## Image preparation

* Download latest version of [Bananian](https://www.bananian.org/):

```Shell
wget http://dl.bananian.org/releases/bananian-latest.zip
```

* Extract archive:

```Shell
unzip bananian-latest.zip
```

* Find SD card device: 
```Shell
dmesg
```
and unmount it
```Shell
mount /dev/sdd
```

* Download image to sd card (remember to change device **/dev/sdd** to correct one):

```Shell
sudo dd if=bananian-1504.img of=/dev/sdd bs=4M
```

* Flash the buffer content to card to make sure everything was written
```Shell
sync
```

Now you image for BananaPi is ready.
 
## Additional software instalation

* Log on you BananaPi with [Putty](http://www.putty.org/) or other ssh client
* Perform repository and system update
* 
```Shell
apt-get update
apt-get upgrate
```

