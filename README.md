# Getting Started

1. install dependencies...
* `cmake`
* `Boost` - required for building python extensions
* `pigpio` - required for python-cpp bindings
* `teensy_loader_cli` - required for loading hex files onto the target.

2. To flash hex files onto the target you will need the `teensy_loader_cli`. Follow installation guidelines [here](https://github.com/PaulStoffregen/teensy_loader_cli). Note that you will may need to compile `teensy_loader_cli` from source and it requires `libusb-0.1.x` as it imports `usb.h` (see [here](https://stackoverflow.com/a/52443692/18890664)). Ensure that you've installed the correct library and header/development files. For example on a `debian` host, this will mean installing `libusb-dev`.

You will also need to create a `udev` rule to mount the device with suitable read/write permissions...
```
# /etc/udev/rules.d/99-teensy.rules

KERNEL=="hiddev*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666"
```

3. Python-cpp bindings rely on the the pigpio library. Once you've installed this, you will have to enable the pigpio daemon '`pigpiod`' by doing the following...
```console
$ sudo systemctl enable pigpiod.service
$ sudo systemctl daemon-reload
```
