'''
Abi Thompson
9-23-22
helper functions used in cv files
'''

from picamera import PiCamera
from picamera.array import PiRGBArray
import time, cv2

'''
Function: camera_init()
Initializes a camera object and sets a constant white balance
Inputs: N/A
Outputs: PiCamera Object
'''
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

'''
Function: cap_and_convert()
Initializes a camera object and sets a constant white balance
Inputs: PiCamera Object
Outputs: Grayscale Image Array
'''
def cap_and_convert(camera):
     # capture image and convert to grayscale
    rawCapture = PiRGBArray(camera)
    camera.capture(rawCapture, format='bgr')
    image = rawCapture.array
    grayscale_img = cv2.cvtColor(image, code=cv2.COLOR_BGR2GRAY)
    return grayscale_img
