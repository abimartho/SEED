#This File is for Demo 1 Code on the Pi
import I2CLCD as LCD
import aruco_location as compVis
import cv_helpers as camInst
import time

lcd = LCD.initializeLCD()
#Aruco Marker Code
camera = camInst.camera_init()

#Size of Aruco Marker in (cm)
dimension = 5

#Loop below angle call with slight delay
while True:
    angle = compVis.aruco_location(camera, dimension)
    writeLCD(str(angle))
    delay(2)

#lcdOff()