import collections
import struct

import pyftdi.serialext

PORT = 'ftdi://ftdi:232h:1/1'
BAUD = 19200
FORMAT = '<llL'

MyData = collections.namedtuple(
    typename='MyData',
    field_names=[
        'timestamp',
        'channel_a',
        'channel_b',
    ],
)

device = pyftdi.serialext.serial_for_url(PORT, baudrate=BAUD)
device.open()
device.reset_input_buffer()
try:
    while True:
        data = MyData(
            *struct.unpack(
                FORMAT,
                device.read(struct.calcsize(FORMAT))
            )
        )
        print(data)
except KeyboardInterrupt:
    pass
finally:
    device.close()