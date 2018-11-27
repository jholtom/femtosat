import socket
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
from struct import *

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
xs = []
ys = []

UDP_IP = "127.0.0.1"
UDP_PORT = 43500

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP,UDP_PORT))

# Two types of frame, 2 = IMU 1 = ACCEL/LIGHT
# Frame 2 looks like this
# and is 54 bytes long
#        char firstName[5];
#        uint16_t count;
#        char frameTypeID;
#        uint8_t frameType;
#        char accelTypeID;
#        uint32_t accelX;
#        uint32_t accelY;
#        uint32_t accelZ;
#        char gyroTypeID;
#        uint32_t gyroX;
#        uint32_t gyroY;
#        uint32_t gyroZ;
#        char magTypeID;
#        uint32_t magX;
#        uint32_t magY;
#        uint32_t magZ;
#        char lastName[6];
# Frame 1 looksl ike this
# and is 46 bytes long
#        char firstName[5];
#        uint16_t count;
#        char timeFieldID;
#        uint64_t time_ms;
#        char frameTypeID;
#        char frameType;
#        char baroTypeID;
#        float altitude;
#        float temp;
#        char lightTypeID;
#        uint32_t red;
#        uint32_t green;
#        uint32_t blue;
#        char lastName[6];

def animate(i,xs,ys):
    data, addr = sock.recvfrom(66)
    (name, count, frameTypeID, frameType, accelTypeID, accelX, accelY, accelZ, gyroTypeID, gyroX, gyroY, gyroZ, magTypeID, magX, magY, magZ, last) = struct.unpack('!4x5sHcBbLLLbLLLbLLL6s8x', data)
    if frameType == 2:
        print "received name: " + name + " " + last
        print "received count: " + str(count)
        print "AccelX: " + str(accelX)
        print "AccelY: " + str(accelY)
        print "AccelZ: " + str(accelZ)
        xs.append(count)
        ys.append(accelX)
        xs = xs[-20:]
        ys = ys[-20:]
        ax.clear()
        ax.plot(xs, ys)

ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=10)
plt.show()
