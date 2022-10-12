'''
Abi Thompson
9-23-22
Function: aruco_quadrant
Input: camera object
Output: int quadrant of image that marker is detected in [0,4], 0 for no marker
        *=====*=====*
        |  2  |  1  |
        *=====*=====*
        |  3  |  4  |
        *=====*=====*
'''

import numpy as np
from picamera import PiCamera
import cv_helpers as chs
import cv2

def aruco_quadrant(camera):
    image = chs.cap_and_convert(camera)
    # load aruco dict
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
    # identify aruco marker
    corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)
    # compute aruco center
    try:
        x = 0
        y = 0
        for i in range(4):
            x += corners[0][0][i][0]
            y += corners[0][0][i][1]
        x = int(x/4)
        y = int(y/4)
    except:
        print('exception')
        return 0
    # determine marker quadrant
    width = image.shape[1]
    height = image.shape[0]
    w_half = width // 2
    h_half = height // 2
    if (x >= w_half) and (y <= h_half):
        return 1
    elif (x < w_half) and (y <= h_half):
        return 2
    elif (x < w_half) and (y > h_half):
        return 3
    else:
        return 4