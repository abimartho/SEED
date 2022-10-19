'''
Abi Thompson
9-23-22
testbench for camera_init and aruco_location
'''
import aruco_quadrant, aruco_location, time, math
import cv_helpers as chs

def cv_testbench():
    camera = chs.camera_init()
    dimension = 5 #cm
    print(aruco_location.aruco_location(camera, dimension) * 180 / math.pi)
        
cv_testbench()