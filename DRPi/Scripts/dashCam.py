#!/usr/bin/python
import picamera
from time import sleep, localtime, strftime
import subprocess

def captureVideo(i):
    if i % 3 == 0:
        return True
    else:
        return False

i = 0
with picamera.PiCamera() as camera:
    stream = picamera.PiCameraCircularIO(camera, seconds=120)
    camera.start_recording(stream, format='h264')
    camera.start_preview()
    try:
        while i < 4:
            camera.annotate_text = 'in loop: %d' % i
            print('in loop')
            print(i)
            camera.wait_recording(10)
            if captureVideo(i):
                time = strftime("%d%b%Y_%H_%M_%S", localtime())
                camera.annotate_text = 'Crash detected!! @ ' + time
                camera.wait_recording(20)
                stream.copy_to('/media/pi/VIDEOSD/crash/%s.h264' % time, seconds=60)
                print time
                out = subprocess.call(["./vidConv.sh", time])
                print out
                stream.clear()
                camera.split_recording(stream)
            i = i+1
    finally:
        camera.stop_recording()
        camera.stop_preview()

