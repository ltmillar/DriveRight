#!/usr/bin/env python

from time import sleep, localtime, strftime
import serial
import subprocess
import picamera

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )

b = True
x = ''
#wait for time from module
while b == False:
    print "waiting for time!"
    x = ser.read(1)
    print x
    if x == 'B': #update time
        x = ser.readline()
        print 'time: ' + x
        out = subprocess.call(["./setClock.sh", x])
        if out == 0:
            b = True
        print out

#record in loop, wait for trigger to record video
while 1:
    with picamera.PiCamera() as camera:
        stream = picamera.PiCameraCircularIO(camera, seconds=300)
        camera.start_recording(stream, format='h264')
        time = strftime("%d%b%Y %H:%M", localtime())
        camera.annotate_text = 'Drive Right!! @ ' + time
        camera.annotate_background = picamera.color.Color('black')        

        camera.start_preview()
        camera.wait_recording(1)
        
        x = ser.read(1)
        print 'ser: ' + x
        
        while x != '':
            x = ser.read(1)
            print 'ser: ' + x
            
            time = strftime("%d%b%Y %H:%M", localtime())
            fileName = strftime("%d%b%Y_%H_%M_%S", localtime())
            
            if x == 'A':   #if aggressive driving event
                camera.annotate_background = picamera.color.Color('red')
                camera.annotate_text = 'Aggressive Driving Detected @ ' + time + ' !!!'
                camera.wait_recording(10)
                stream.copy_to('/media/pi/VIDEOSD/aggressive/%s.h264' % fileName, seconds=20)
                subprocess.call(["./aggVidConv.sh", fileName])

                while x != '':
                    x = ser.read(1)
                    print 'ser: ' + x
                
                
            elif x == 'C': #if crash event
                camera.annotate_background = picamera.color.Color('red')
                camera.annotate_text = 'CRash Event Detected @ ' + time + ' !!!'
                camera.wait_recording(120)
                stream.copy_to('/media/pi/VIDEOSD/crash/%s.h264' % fileName, seconds=300)
                subprocess.call(["./vidConv.sh", fileName])
            
                while x != '':
                    x = ser.read(1)
                    print 'ser: ' + x
            
