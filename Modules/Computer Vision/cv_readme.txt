================================================================================
                   Marker Detection Scheme Documentation
--------------------------------------------------------------------------------
                     EENG 350 | 10-2-22 | Abi Thompson
================================================================================

The marker detection scheme is encapsulated by one class:
+ SeedCV
There is also a testbench function used to test the operation of the class and
the functions within, but that is not used in the project.
--------------------------------------------------------------------------------
To use:
1. Import the python file to the project
2. Create an instantiation of the SeedCV object
    example: cv = SeedCV.SeedCV()
3. Call functions from the class object as needed
    example: angle, distance = cv.aruco_location()
Details on the public class methods are included in the next section 
--------------------------------------------------------------------------------
SeedCV.aruco_quadrant()
    params: none
    returns: integer corresponding to image quadrant
    function: detects 6x6 aruco marker and determines its quadrant in the image.
        Returns an integer corresponding to that quadrant. The integers
        corresponding to each quadrant are:
            0: no marker in image
            *=====*=====*
            |  2  |  1  |
            *=====*=====*
            |  3  |  4  |
            *=====*=====*
        An example of use is:
            quad = cv.aruco_quadrant()
            
 SeedCV.aruco_location()
    params: none
    returns: angle from vertical of the marker in rad, distance of marker from
        camera in ft
    function: detects 6x6 aruco marker and determines its location with respect
        to the camera. Returns the angle of the marker from the vertical and
        the distance of the marker from the camera.
        An example of use is:
            theta, distance = cv.aruco_location()
    
    
