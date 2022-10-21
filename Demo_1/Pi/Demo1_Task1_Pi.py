#This File is for Demo 1 Code on the Pi
import I2CLCD as LCD
import aruco_location as compVis
import cv_helpers as camInst
import time

LCD.initializeLCD()
#Aruco Marker Code
camera = camInst.camera_init()
#Loop below angle call with slight delay
while True:
    angle = compVis.aruco_location(camera)
    writeLCD(angle)
    delay(2)

#lcdOff()