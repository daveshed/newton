import collections
import queue
import struct
import sys
import threading
import time

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
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
QUEUE = queue.Queue()
X_DATA = []
Y_DATA = []

def checksum(chunk):
    result = 0
    for x in chunk[:-1]:
        result += x
        result %= 256
    return result

def handle_chunk(chunk):
    data = MyData(*struct.unpack(FORMAT, chunk))
    if checksum(chunk) != data.checksum:
        raise AssertionError("Data corruption")
    QUEUE.put(data)


def worker(condition):
    device = pyftdi.serialext.serial_for_url(PORT, baudrate=BAUD, timeout=0)
    device.open()
    # clear the input buffer...
    print("Clearing buffer...")
    while device.read():
        pass
    print("Listening...")
    data = b''
    try:
        while condition():
            data += device.read(BUFFER_SIZE)
            if len(data) >= CHUNK_SIZE:
                chunk = data[:CHUNK_SIZE]
                handle_chunk(chunk)
                data = data[CHUNK_SIZE:]
    except KeyboardInterrupt:
        print("Quitting worker...")
    finally:
        device.close()

def animate(_):
    while True:
        try:
            data = QUEUE.get(block=False)
            seconds, mass = calibrate(data)
            X_DATA.append(seconds)
            Y_DATA.append(mass)
        except queue.Empty:
            break
    plt.cla()
    plt.plot(X_DATA[-100:], Y_DATA[-100:])


def save_data(filename):
    print(f"Saving data to {filename}")
    with open(filename, 'w') as f:
        for x, y in zip(X_DATA, Y_DATA):
            f.write(f"{x}\t{y}\n")


def main(argv: list = None):
    if argv is None:
        argv = sys.argv[1:]
    try:
        [filename] = argv
    except:
        filename = 'testing.dat'
    # data should be put into the queue
    run = threading.Event()
    run.set()
    thread = threading.Thread(target=worker, args=(run.is_set,))
    thread.start()
    # to quit the animation, press 'q'.
    animation = FuncAnimation(plt.gcf(), animate, interval=100)
    plt.show()
    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Quitting...")
        run.clear()
        thread.join()
        save_data(filename)

if __name__ == '__main__':
    main()
