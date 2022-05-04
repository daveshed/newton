# Getting Started

1. install dependencies...
* `cmake`
* `arduino` - contains SDK
* `arduino-avr-core` - contains more avr tools
* `Boost` - required for building python extensions
* `pigpio` - required for python-cpp bindings

2. [Optional] You can set the SDK path for cmake if necessary. The default path is given below so you shouldn't need to do this...
```console
$ cmake -DARDUINO_SDK_PATH=/usr/share/arduino/ ../
```
The hardware platform path may also need to be set. This is demonstrated in the ci.sh file. You can do it with the `register_hardware_platform` directive.

3. Python-cpp bindings rely on the the pigpio library. Once you've installed this, you will have to enable the pigpio daemon '`pigpiod`' by doing the following...
```console
$ sudo systemctl enable pigpiod.service
$ sudo systemctl daemon-reload
```