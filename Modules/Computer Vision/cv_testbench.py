'''
Abi Thompson
11-1-22
testbench for testing operation 
'''
import SeedCV, time

def cv_testbench():
    cv = SeedCV.SeedCV()
    '''
    while True:
        print(cv.aruco_location())
        time.sleep(2)
    '''
    theta, d_ft = cv.find_marker(2)
    print(theta)
    print(d_ft)
        
cv_testbench()