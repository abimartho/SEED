'''
Abi Thompson
9-23-22
function to initialize picamera and adjust white balance
'''

from picamera import PiCamera
from picamera.array import PiRGBArray
import time

def camera_init():
    # init camera object
    camera = PiCamera()
    # wait to let white balance settle
    time.sleep(2)
    # query current white balance value and set that as constant
    gains = camera.awb_gains
    camera.awb_mode = 'off'
    camera.awb_gains = gains
    # return camera object
    return camera