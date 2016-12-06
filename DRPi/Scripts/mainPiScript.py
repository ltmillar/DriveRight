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
    with picamera.PiCamera() as camera:
        stream = picamera.PiCameraCircularIO(camera, seconds=300)
        camera.start_recording(stream, format='h264')
        time = strftime("%d%b%Y %H:%M", localtime())
        camera.annotate_text = 'Drive Right!! @ ' + time

        x = ser.read(1)
        if x != ''
            time = strftime("%d%b%Y %H:%M", localtime())
            fileName = strftime("%d%b%Y_%H_%M_%S", localtime())
            
            if x == 'A':   #if aggressive driving event
                camera.annotate_text = 'Aggressive Driving Detected @ ' + time + ' !!!'
                camera.wait_recording(20)
                stream.copy_to('/media/pi/VIDEOSD/aggressive/%s.h264' % fileName, seconds=60)

                
            else if: x == 'C': #if crash event
                camera.annotate_text = 'CRash Event Detected @ ' + time + ' !!!'
                camera.wait_recording(120)
                stream.copy_to('/media/pi/VIDEOSD/crash/%s.h264' % fileName, seconds=300)

            
            
            
