#This File is for Demo 1 Code on the Pi
import I2CLCD as LCD
import aruco_location as compVis
import cv_helpers as camInst
import time


LCD.initializeLCD()
#Aruco Marker Code
camera = camInst.camera_init()

#Size of Aruco Marker in (cm)
dimension = 5

#Loop below angle call with slight delay
while True:
    angle = compVis.aruco_location(camera, dimension)
    LCD.writeLCD(str(angle))
    time.sleep(2)

#lcdOff()