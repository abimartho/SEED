# -*- coding: utf-8 -*-
"""
Created on Mon Sep 26 10:54:02 2022

@author: Samuel Wicklund
@org: Colorado School of Mines Electrical Engineering
@email: swicklund@mines.edu
"""

from cmath import pi
import serial
import time
import board
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

import aruco_location as camLoc
import camera_init

#Function to read serial for LCD
def ReadfromArduino():
    try:
        currentPosition = ser.readline().decode("utf-8").rstrip()
        #current_val = int.from_bytes(currentPosition, "big")
        print("serial output : ", currentPosition)
    except:
        print("Read Error")
        
def SendToArduino(camera):
    try:
        setPoint = camLoc.aruco_location(camera)
        sendPoint = setPoint.to_bytes(1, "big")
        ser.write(sendPoint)
    except:
        print("Something went Wrong")
    return setPoint


#LCD Setup
lcd_columns = 16
lcd_rows = 2
i2c = board.I2C()
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)

#Set Initial LCD Values
lcd.clear()
lcd.color = [0, 100, 0]

#Set address
ser = serial.Serial('/dev/ttyACM0', 115200)
#Wait for connection to complete
time.sleep(5)

#Setup Camera
camera = camera_init.camera_init()
setPoint = SendToArduino(camera)

while True:
    try:
        ReadfromArduino()
        #currPosition = (ReadfromArduino()/40) * pi
        #lcd.message = "Set Point: " + str(setPoint) + "\nPosition: " + str(currPosition)
    except IOError:
        lcd.message = "Oops! IOError, check Pi to Arduino USB Connections"

lcd.color = [0, 0, 0]
lcd.clear()