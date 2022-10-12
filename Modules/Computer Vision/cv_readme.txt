================================================================================
                   Marker Detection Scheme Documentation
--------------------------------------------------------------------------------
                     EENG 350 | 9-26-22 | Abi Thompson
================================================================================

The marker detection scheme is composed of two primary functions:
+ camera_init()
+ aruco_location()
There is also a testbench function used to test the operation of the function
but that is not used in the operation of the full project.

--------------------------------------------------------------------------------
To use:
Run camera_init() once to initialize the piCamera object and set a constant
white balance. Camera init returns a camera object. An example of use is:
    camera = camera_init()
The function aruco_location can be ran repeatedly. It takes the parameter of
the camera object and returns an integer [0,4] corresponding to the quadrant of 
the image that a 6x6 aruco marker is detected in. The integers corresponding to
each quadrant are:
    0: no marker in image
    *=====*=====*
    |  2  |  1  |
    *=====*=====*
    |  3  |  4  |
    *=====*=====*
An example of use is:
    quad = aruco_location(camera)
    
--------------------------------------------------------------------------------
camera_init()
    params: none
    returns: piCamera object
    function: initializes a piCamera object and sets a constant white balance.

aruco_location(camera)
    params: piCamera object camera
    returns: integer corresponding to image quadrant
    function: detects 6x6 aruco marker and determines its quadrant in the image.
    Returns an integer corresponding to that quadrant.
    
    
