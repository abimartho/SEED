'''
Abi Thompson
11-1-22
testbench for testing operation 
'''
import SeedCV, time

def cv_testbench():
    cv = SeedCV.SeedCV()
    while True:
        print(cv.aruco_location())
        time.sleep(2)
        
cv_testbench()