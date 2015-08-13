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
* Configure system
```Shell
bananian-config
bananian-update
```
* Perform repository and system update
```Shell
apt-get update
apt-get upgrate
```

* Install [Webmin](http://www.webmin.com/)
```Shell
apt-get install perl libnet-ssleay-perl openssl libauthen-pam-perl libpam-runtime libio-pty-perl apt-show-versions python
wget http://sourceforge.net/projects/webadmin/files/webmin/1.760/webmin_1.760_all.deb
dpkg --install webmin_1.760_all.deb
```

* Install [Plex](https://plex.tv/)

Details how to install Plex on Banana Pi are described here: http://www.htpcguides.com/install-plex-media-server-on-banana-pi-with-bananian/

On my site sudo  was not installed so I need to modyfy scripts.

```Shell
wget -O - https://dev2day.de/pms/dev2day-pms.gpg.key | apt-key add -
echo "deb https://dev2day.de/pms/ wheezy main" | tee /etc/apt/sources.list.d/pms.list
apt-get update
apt-get install plexmediaserver -y
```

[Plex](https://plex.tv/) should be now available at: [http://banana-ip:32400/web](http://banana-ip:32400/web)

* Configure SSH to access via private key
 + First generate SSH key with [PuTTyGen](https://winscp.net/eng/docs/ui_puttygen) which is available together with [Putty](http://www.putty.org/)
 + Save both keys and copy content of public key to ***/root/.ssh/authorized_keys*** file on banana pi

* Install [dnsmasq](https://en.wikipedia.org/wiki/Dnsmasq)

```Shell
apt-get install dnsmasq
``` 

and copy:

```Shell
domain-needed
bogus-priv

domain=salwatorska.pl
expand-hosts
local=/salwatorska.pl/ 

listen-address=127.0.0.1 
listen-address=192.168.1.99
listen-address=192.168.2.99
bind-interfaces

# serve DHCP on guest network
dhcp-range=eth0.1,192.168.2.100,192.168.2.254,24h
dhcp-option=eth0.1,option:router,192.168.2.1
dhcp-option=eth0.1,option:dns-server,192.168.2.99,8.8.8.8
dhcp-option=eth0.1,252,"\n"

# serve DHCP on private network
dhcp-range=eth0.2,192.168.1.100,192.168.1.254,24h
dhcp-option=eth0.2,option:router,192.168.1.1
dhcp-option=eth0.2,option:dns-server,192.168.1.99,8.8.8.8
dhcp-option=eth0.2,252,"\n"

# DNS servers to forward requests to 
server=74.82.42.42
server=208.67.222.222
server=217.17.34.10


dhcp-host=00:b2:00:00:5e:5c, KameraWejscie, 192.168.1.51
dhcp-host=00:0f:7c:02:5a:0a, KameraParter, 192.168.1.52
dhcp-host=00:12:12:39:72:ca, KameraPietro1, 192.168.1.53
dhcp-host=00:12:12:39:9c:78, KameraPietro2, 192.168.1.54

# enable TFTP 
enable-tftp
tftp-root=/var/ftpd

domain-needed
cache-size=4096
log-queries
log-async
#log-dhcp
#
#

```
