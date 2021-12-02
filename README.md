
https://create.arduino.cc/projecthub/b45i/getting-started-with-arduino-cli-7652a5

using arduino-cli
1. check that the board is detected with:
```
$ arduino-cli board list

Port         Protocol Type              Board Name  FQBN            Core
/dev/ttyACM0 serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
/dev/ttyS0   serial   Unknown

```
2. Install the required core and dependencies...
```
$ arduino-cli core install arduino-avr
$ arduino-cli lib install "HX711 Arduino Library"
```
3. Go to this directory and build like this...
```
$ arduino-cli compile -b arduino:avr:uno
```
4. Then upload...
```
$ arduino-cli upload --log-level info -vtp /dev/ttyACM0 -b arduino:avr:uno
```