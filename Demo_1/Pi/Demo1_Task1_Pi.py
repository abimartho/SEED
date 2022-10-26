#This File is for Demo 1 Code on the Pi
import I2CLCD as LCD #Takes in team made I2C and LCD module
import aruco_location as compVis #Defines team aruco marker calculator
import cv_helpers as camInst #Defines team made camera helper function for aruco_location
import time

#Initialize LCD
LCD.initializeLCD()

#Call camInst to define camera setup
camera = camInst.camera_init()

#Size of Aruco Marker in (cm)
dimension = 5

#while True loop so that the angle from the camera is always being read in, then written onto the LCD. 
#Sleep for two seconds after each run through to ensure that the LCD has time to be fully written to and so there is a break inbetween each camera read
while True:
    angle = compVis.aruco_location(camera, dimension) #Call function to calculate angle providing camera object and marker size
    LCD.writeLCD(str(angle)) #Turn angle into string and send to writeLCD function to display on LCD
    time.sleep(2) #Sleep for 2 seconds and then repeat loop.
