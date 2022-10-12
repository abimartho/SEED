'''
Abi Thompson
9-23-22
testbench for camera_init and aruco_location
'''
import aruco_quadrant, time
import cv_helpers as chs

def cv_testbench():
    camera = chs.camera_init()
    while True:
        print(aruco_quadrant.aruco_quadrant(camera))
        time.sleep(1)
        
cv_testbench()