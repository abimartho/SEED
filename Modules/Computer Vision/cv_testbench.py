'''
Abi Thompson
9-23-22
testbench for camera_init and aruco_location
'''
import aruco_quadrant, aruco_location, time
import cv_helpers as chs

def cv_testbench():
    #camera = chs.camera_init()
    camera = None
    dimension = 5 #cm
    while True:
        aruco_location.aruco_location(camera, dimension)
        time.sleep(1)
        
cv_testbench()