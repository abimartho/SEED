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
import threading

startTime = time.time()

def DoMath(currPos, valTime):
    sVal = str(currPos) + " " + str(valTime) + "\n"
    print(sVal)
    valueRet = (int(currPos)/1590 * pi)
    return valueRet
#Function to read serial for LCD
def ReadfromArduino():
    ser.reset_input_buffer()
    currentPosition = ser.readline().decode("utf-8").rstrip()
    valueTime = int((time.time() - startTime) * 1000)
    mathValue = DoMath(currentPosition, valueTime)
    return mathValue
        
def SendToArduino(camera):
    try:
        setPoint = camLoc.aruco_location(camera)
        sendPoint = setPoint.to_bytes(1, "big")
        ser.write(sendPoint)
    except:
        print("I couldn't send")
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
print("Sleeping 2 seconds to give Arduino time to send something back")
time.sleep(2)

while True:
    try:
        currPosition = ReadfromArduino()
        #print("Start of LCD: ",time.time())
        #This takes so gosh darn long
        lcd.message = "Set Point: " + str(setPoint) + "\nPosition: " + str(currPosition)
        #print("End of LCD: ", time.time())
    except KeyboardInterrupt:
        print("Closing File")
        lcd.color = [0, 0, 0]
        lcd.clear()
        print("File Closed")
    except IOError:
        lcd.message = "Oops! IOError, check Pi to Arduino USB Connections"
