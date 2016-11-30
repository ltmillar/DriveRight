#!/usr/bin/env python

import time
import serial

ser = serial.Serial(
    port='/dev/tty1',
    baudrate=19200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=10
    )

counter = 0

while counter < 10:
    x = ser.readline();
    if x != '':
        print x
        time.sleep(1)
    else:
        print 'Counter = ' + str(counter)
    counter += 1
