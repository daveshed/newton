import collections
import struct
import time

import matplotlib.pyplot as plt
import pyftdi.serialext

PORT = 'ftdi://ftdi:232h:1/1'
BAUD = 19200
FORMAT = '<LllB'
CHUNK_SIZE = struct.calcsize(FORMAT)
BUFFER_SIZE = 64

MyData = collections.namedtuple(
    typename='MyData',
    field_names=[
        'timestamp',
        'channel_a',
        'channel_b',
        'checksum',
    ],
)
X_DATA = []
Y_DATA = []

def checksum(chunk):
    result = 0
    for x in chunk[:-1]:
        result += x
        result %= 256
    return result

def handle_chunk(chunk):
    print(chunk)
    data = MyData(*struct.unpack(FORMAT, chunk))
    print(data)
    if checksum(chunk) != data.checksum:
        raise AssertionError("Data corruption")
    X_DATA.append(data.timestamp)
    Y_DATA.append(data.channel_a)

def consume(queue):
    if len(queue) >= CHUNK_SIZE:
        chunk = queue[:CHUNK_SIZE]
        handle_chunk(chunk)
        return queue[CHUNK_SIZE:]
    return queue

# non-blocking
device = pyftdi.serialext.serial_for_url(PORT, baudrate=BAUD, timeout=0)
device.open()
# clear the input buffer...
print("Clearing buffer...")
while device.read():
    pass

queue = b''

try:
    print("Listening...")
    while True:
        queue += device.read(BUFFER_SIZE)
        queue = consume(queue)
except KeyboardInterrupt:
    print("Quitting...")
finally:
    device.close()
plt.scatter(X_DATA, Y_DATA)
plt.show()
