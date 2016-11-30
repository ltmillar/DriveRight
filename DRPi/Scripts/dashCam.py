#!/usr/bin/python
import picamera
from time import sleep

def captureVideo(i):
    if i % 3 == 0:
        return True
    else:
        return False

i = 0
with picamera.PiCamera() as camera:
    stream = picamera.PiCameraCircularIO(camera, seconds=10)
    camera.start_recording(stream, format='h264')
    camera.start_preview()
    try:
        while i < 10:
            camera.annotate_text = 'in loop: %d' % i
            print('in loop')
            print(i)
            camera.wait_recording(5)
            if captureVideo(i):
                stream.copy_to('/media/pi/VIDEOSD/crash/%d.h264' % i, seconds=15)
                stream.clear()
                camera.split_recording(stream)
            i = i+1
    finally:
        camera.stop_recording()
        camera.stop_preview()

