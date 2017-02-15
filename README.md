# Wall Of Love
## App
### Launch
`python app.py`
### Exit
`x`
### Restart
`i`

## Raspberry
### login
```
login: pi
passwd: raspberry
```
### wifi
#### Guest
`# sudo nano /etc/wpa_supplicant/wpa_supplicant.conf`
```
network={
    ssid="MR_Guest"
    psk="XXX"
}
```
```
# sudo ifdown wlan0 && sudo ifup wlan0
# ifconfig
```

## Images
### Backup
```
# diskutil list
# diskutil unmountDisk /dev/disk2
# sudo dd if=/dev/disk2 of=old_pi.img bs=1m
```
### Restore
```
# diskutil list
# diskutil unmountDisk /dev/disk2
# sudo dd if=Downloads/2017-01-11-raspbian-jessie-lite.img of=/dev/disk2 bs=1m
# diskutil unmountDisk /dev/disk2
```

## Troubleshoots

### Keyboard
##### If pipe on keyboard not working, then edit `/etc/default/keyboard` with this values
```
XKBMODEL=”pc105”
XKBLAYOUT=”us”
```
