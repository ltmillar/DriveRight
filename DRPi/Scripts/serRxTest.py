#!/usr/bin/env python

import time
import serial

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )

counter = 0

while counter < 100:
    x = ser.read(1);

    if x != '':
        print x
        x = ser.readline()
        print x
        x = ''
        time.sleep(1)
    else:
        print 'Counter = ' + str(counter)
    counter += 1
