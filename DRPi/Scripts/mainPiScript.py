#!/usr/bin/env python

import time
import serial
import subprocess

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )

x == ''
#wait for time from module
while x == '':
    x = ser.read(1)
    if x == 'B': #update time
        x = ser.readline()
        subprocess.call(["./setClock.sh", x])

#record in loop, wait for trigger to record video
while 1:
    x = ser.read(1)
    if x == 'A':   #if aggressive driving event

    else if: x == 'C': #if crash event
    
