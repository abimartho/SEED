'''
SeedCV class that encompasses the project's computer vision
Author: Abi Thompson
Date: 10/31/22
'''

import time, cv2, math
import numpy as np
#from picamera import PiCamera
#from picamera.array import PiRGBArray

class SeedCV:
    #==========================================================================
    #                             CLASS VARIABLES
    #==========================================================================
    camera = None
    marker_dimension = 5 # cm

    #==========================================================================
    #                             CLASS FUNCTIONS
    #==========================================================================
    '''
    Function: __init__()
    Initializes the class upon instantiation
    '''
    def __init__(self):
        self.camera = self.camera_init()
    
    '''
    Function: camera_init()
    Initializes a camera object and sets a constant white balance
    Inputs: N/A
    Outputs: PiCamera Object
    '''
    def camera_init(self):
        '''
        # init camera object
        camera = PiCamera()
        # wait to let white balance settle
        time.sleep(2)
        # query current white balance value and set that as constant
        gains = camera.awb_gains
        camera.awb_mode = 'off'
        camera.awb_gains = gains
        # return camera object
        '''
        camera = None
        return camera
    
    '''
    Function: cap_and_convert()
    Initializes a camera object and sets a constant white balance
    Inputs: N/A
    Outputs: Grayscale Image Array
    '''
    def cap_and_convert(self):
        '''
        # capture image and convert to grayscale
        rawCapture = PiRGBArray(self.camera)
        self.camera.capture(rawCapture, format='bgr')
        image = rawCapture.array
        grayscale_img = cv2.cvtColor(image, code=cv2.COLOR_BGR2GRAY)
        '''
        grayscale_img = None
        return grayscale_img
    
    '''
    Function: distance()
    Inputs: 2 two-dimensional points, p1 and p2
    Outputs: Distance between the two points as a float
    '''
    def distance(self, p1, p2):
        t0 = (p1[0] - p2[0])**2
        t1 = (p1[1] - p2[1])**2
        return math.sqrt(t0+t1)
        
    '''
    Function: center()
    Computes the center of the aruco marker
    Inputs: The corners aray as computed by detect markers
    Outputs: The center of the first marker in the list as coordinates (x, y)
    '''
    def center(self, corners):
        # compute aruco center
        try:
            x = 0
            y = 0
            for i in range(4):
                x += corners[0][0][i][0]
                y += corners[0][0][i][1]
            x = int(x/4)
            y = int(y/4)
            return (x, y)
        except:
            print('exception')
            return 0

    '''
    Function: center_single()
    Computes the center of the aruco marker
    Inputs: The corners aray as computed by detect markers
    Outputs: The center of the first marker in the list as coordinates (x, y)
    '''
    def center_single(self, corners):
        # compute aruco center
        x = 0
        y = 0
        for i in range(4):
            x += corners[i][0]
            y += corners[i][1]
        x = int(x/4)
        y = int(y/4)
        return (x, y)

    
    '''
    Function: aruco_quadrant
    Input: N/A
    Output: int quadrant of image that marker is detected in [0,4], 0 for no marker
            *=====*=====*
            |  2  |  1  |
            *=====*=====*
            |  3  |  4  |
            *=====*=====*
    '''
    def aruco_quadrant(self):
        image = self.cap_and_convert()
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

    '''
    Function: aruco_location
    Input: N/A
    Output: Angle from vertical of the marker in radians, distance of marker from camera in ft
            Returns None if no markers detected in image
    '''
    def aruco_location(self):
        # capture image and detect aruco marker
        image = self.cap_and_convert()
        #image = cv2.imread('test_images/img3.jpg') 
        aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
        corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)
        
        # parameters and constants
        fov_x = 53.50 * math.pi / 360 # converted to radians and halved
        width_pix = image.shape[1]
        cx = width_pix // 2

        try:
            # compute marker's distance from camera
            aruco_pix = self.distance(corners[0][0][0], corners[0][0][1])
            width_cm = ( self.marker_dimension * width_pix) / aruco_pix
            half_width_cm = width_cm / 2
            d = half_width_cm / math.tan(fov_x)

            # convert cm to ft for the return value
            cm_per_ft = 30.48
            d_ft = d / cm_per_ft

            # use this distance and marker center to find angle from cam
            center = self.center(corners)
            ca = int(center[0])
            h_pix = ca - cx
            h_cm = width_cm * h_pix / width_pix
            theta = math.atan( h_cm / d )
            return theta, d_ft
        except:
            # no markers detected
            return None, None

    '''
    Function: find_marker
    Input: id of target marker
    Output: Angle from vertical of the marker in radians, distance of marker from camera in ft
            Returns None if that marker not detected in the image
    '''
    def find_marker(self, id):
        # capture image and detect aruco marker
        #image = self.cap_and_convert()             # demos use 4X4 markers
        image = cv2.imread('test_images/img2.jpg')  # test images use 6X6 markers
        aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
        corners, ids, temp = cv2.aruco.detectMarkers(image, aruco_dict)

        # marker not in view
        if id not in ids:
            return None, None

        # get index of id in list and find corresponding corners
        temp = np.where(ids==id)
        idx = [int(temp[0]), int(temp[1])]
        target_corners = corners[idx[0]][idx[1]]

        # parameters and constants
        fov_x = 53.50 * math.pi / 360 # converted to radians and halved
        width_pix = image.shape[1]
        cx = width_pix // 2

        # compute marker's distance from camera
        aruco_pix = self.distance(target_corners[0], target_corners[1])
        width_cm = ( self.marker_dimension * width_pix) / aruco_pix
        half_width_cm = width_cm / 2
        d = half_width_cm / math.tan(fov_x)

        # convert cm to ft for the return value
        cm_per_ft = 30.48
        d_ft = d / cm_per_ft

        # use this distance and marker center to find angle from cam
        center = self.center_single(target_corners)
        ca = int(center[0])
        h_pix = ca - cx
        h_cm = width_cm * h_pix / width_pix
        theta = math.atan( h_cm / d )
        return theta, d_ft