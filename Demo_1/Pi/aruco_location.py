'''
Abi Thompson
10-12-22
Function: aruco_location
Input: camera object, marker dimension in cm
Output: Angle from vertical of the marker in radians
        Returns None if no markers detected in image
'''

import numpy as np
from picamera import PiCamera
import cv_helpers as chs
import cv2, math

def aruco_location(camera, dim):
    # capture image and detect aruco marker
    image = chs.cap_and_convert(camera)
    #image = cv2.imread('test_images/img3.jpg') 
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
    corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)
    
    # parameters and constants
    fov_x = 53.50 * math.pi / 360 # converted to radians and halved
    width_pix = image.shape[1]
    cx = width_pix // 2

    try:
        # compute marker's distance from camera
        aruco_pix = chs.distance(corners[0][0][0], corners[0][0][1])
        width_cm = ( 5 * width_pix) / aruco_pix
        half_width_cm = width_cm / 2
        d = half_width_cm / math.tan(fov_x)

        # use this distance and marker center to find angle from cam
        center = chs.center(corners)
        ca = int(center[0])
        h_pix = ca - cx
        h_cm = width_cm * h_pix / width_pix
        theta = math.atan( h_cm / d )
        return theta
    except:
        # no markers detected
        return None
    