'''
Abi Thompson
9-23-22
testbench for camera_init and aruco_location
'''
import camera_init, aruco_location, time

def cv_testbench():
    camera = camera_init.camera_init()
    while True:
        aruco_location.aruco_location(camera)
        time.sleep(1)
        
cv_testbench()