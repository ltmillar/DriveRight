from picamera import PiCamera
from time import sleep

camera = PiCamera()
camera.resolution = (64, 64)
camera.start_preview()
camera.start_recording('/home/pi/video.h264')
sleep(10)
camera.stop_recording()
camera.stop_preview()
