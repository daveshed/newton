import collections
import queue
import struct
import sys
import threading
import time

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import serial

PORT = '/dev/ttyACM0'
BAUD = 9600
FORMAT = '<LflB'
CHUNK_SIZE = struct.calcsize(FORMAT)
BUFFER_SIZE = 64

MyData = collections.namedtuple(
    typename='MyData',
    field_names=[
        "timestamp",
        "force_x",
        "data_x",
        "checksum",
    ],
)
QUEUE = queue.Queue()
T_DATA = []
X_DATA = []
OFFSETS = None


def checksum(chunk):
    result = 0
    for x in chunk[:-1]:
        result += x
        result %= 256
    return result


def handle_chunk(chunk):
    data = MyData(*struct.unpack(FORMAT, chunk))
    print(data)
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
    device = serial.Serial(PORT, BAUD, timeout=0)
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
            T_DATA.append(data.timestamp)
            if OFFSETS is None:
                OFFSETS = data.data_x
            X_DATA.append(data.data_x - OFFSETS)
        except queue.Empty:
            break
    plt.cla()
    plt.plot(T_DATA[-100:], X_DATA[-100:])

def save_data(filename):
    print(f"Saving data to {filename}")
    with open(filename, 'w') as f:
        for t, x in zip(T_DATA, X_DATA):
            f.write(f"{t}\t{x}\n")


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
