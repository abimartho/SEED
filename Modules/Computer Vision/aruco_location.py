'''
Abi Thompson
10-12-22
Function: aruco_location
Input: camera object
Output: The world coordinates and angle from vetical of the marker
'''

import numpy as np
from picamera import PiCamera
import cv_helpers as chs
import cv2

def aruco_location(camera)
    # capture image and detect aruco marker
    image = chs.cap_and_convert(camera)
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
    corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)
    
    # initialize camera intrinsic parameter matrix
    f = 3.6 #mm
    
    # compute pose
    #rvecs,tvecs,_ = cv2.aruco.estimatePoseSingleMarkers(corners, MARKER_LENGTH, K, dist_coeff)
    
    # yank that angle