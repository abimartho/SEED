'''
Abi Thompson
9-23-22
helper functions used in cv files
'''

from picamera import PiCamera
from picamera.array import PiRGBArray
import time, cv2, math

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

'''
Function: distance()
Inputs: 2 two-dimensional points, p1 and p2
Outputs: Distance between the two points as a float
'''
def distance(p1, p2):
     t0 = (p1[0] - p2[0])**2
     t1 = (p1[1] - p2[1])**2
     return math.sqrt(t0+t1)
    
'''
Function: center()
Computes the center of the aruco marker
Inputs: The corners aray as computed by detect markers
Outputs: The center of the first marker in the list as coordinates (x, y)
'''
def center(corners):
    # compute aruco center
    try:
        x = 0
        y = 0
        for i in range(4):
            x += corners[0][0][i][0]
            y += corners[0][0][i][1]
        x = int(x/4)
        y = int(y/4)
        return (x, y)
    except:
        print('exception')
        return 0

