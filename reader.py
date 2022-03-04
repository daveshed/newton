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
FORMAT = '<LffflllB'
CHUNK_SIZE = struct.calcsize(FORMAT)
BUFFER_SIZE = 64

MyData = collections.namedtuple(
    typename='MyData',
    field_names=[
        "timestamp",
        "force_x",
        "force_y",
        "force_z",
        "data_x",
        "data_y",
        "data_z",
        "checksum",
    ],
)
QUEUE = queue.Queue()
T_DATA = []
X_DATA = []
Y_DATA = []
Z_DATA = []
OFFSETS = None
# calibration
# 0kg    68300
# 10kg  510100
# 20kg  950110
# 35kg  163400


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

def calibrate(data):
    m = 44714
    c = 64027
    mass = (data.channel_a - c) / m
    seconds = data.timestamp / 1000
    return seconds, mass


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
    global OFFSETS
    while True:
        try:
            data = QUEUE.get(block=False)
            # seconds, mass = calibrate(data)
            # T_DATA.append(seconds)
            # Y_DATA.append(mass)
            T_DATA.append(data.timestamp)
            if OFFSETS is None:
                OFFSETS = data.data_x, data.data_y, data.data_z
            X_DATA.append(data.data_x - OFFSETS[0])
            Y_DATA.append(data.data_y - OFFSETS[1])
            Z_DATA.append(data.data_z - OFFSETS[2])
        except queue.Empty:
            break
    plt.cla()
    plt.plot(T_DATA[-100:], X_DATA[-100:])
    plt.plot(T_DATA[-100:], Y_DATA[-100:])
    plt.plot(T_DATA[-100:], Z_DATA[-100:])

def save_data(filename):
    print(f"Saving data to {filename}")
    with open(filename, 'w') as f:
        for t, x, y, z in zip(T_DATA, X_DATA, Y_DATA, Z_DATA):
            f.write(f"{t}\t{x}\t{y}\t{z}\n")


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
