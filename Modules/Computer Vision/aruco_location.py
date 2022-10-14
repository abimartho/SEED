'''
Abi Thompson
10-12-22
Function: aruco_location
Input: camera object, marker dimension in cm
Output: The world coordinates and angle from vetical of the marker
'''

import numpy as np
from picamera import PiCamera
import cv_helpers as chs
import cv2

def aruco_location(camera, dim):
    # capture image and detect aruco marker
    #image = chs.cap_and_convert(camera)
    image = cv2.imread('test_images/img0.jpg')
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
    corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)
    
    # initialize camera intrinsic parameter matrix
    f = 3.6 #mm
    cx = image.shape[1] // 2
    cy = image.shape[0] // 2
    K = np.array([[f, 0, cx], [0, f, cy], [0, 0, 1]])
    dim /= 100 # convert dim from cm to meters
    
    # compute pose
    rvecs,tvecs,_ = cv2.aruco.estimatePoseSingleMarkers(corners, dim, K, np.array([]))
    try:
        cv2.drawFrameAxes(image, K, np.array([]), rvecs, tvecs, dim//2)
        cv2.imshow('image', image)
        cv2.waitKey(0)
    except:
        # no markers in image
        return None
    
    # yank that angle