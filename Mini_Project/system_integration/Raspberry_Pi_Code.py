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

prevSetPoint = 5; #Holds previous setpoint so Arduino doesn't get a new read command until value changes.

#LCD Setup
lcd_columns = 16
lcd_rows = 2
i2c = board.I2C()
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)

#Set Initial LCD Values
lcd.clear()
lcd.color = [0, 100, 0]


startTime = time.time()

def DoMath(currPos, valTime):
    #sVal = str(currPos) + " " + str(valTime) + "\n"
    #print(sVal)
    valueRet = ((int(currPos)/1600) * pi)
    return valueRet

#Function to read serial for LCD
def ReadfromArduino():
    mathValue = 0
    currentPosition = ser.readline().decode("utf-8").rstrip()
    if currentPosition != '':
        valueTime = int((time.time() - startTime) * 1000)
        mathValue = DoMath(currentPosition, valueTime)
    return mathValue
       
def SendToArduino(camera):
    setPoint = camLoc.aruco_location(camera)
    #setPoint = 2
    print(setPoint)
    ser.write(str(setPoint).encode("utf-8"))
    ser.flush()
    ser.reset_input_buffer()
    return setPoint



#Set address
ser = serial.Serial('/dev/ttyACM0', 115200, timeout = 1)
ser.reset_output_buffer()
ser.flush()
time.sleep(2)

#Setup Camera
camera = camera_init.camera_init()
i = 0
while True:
    try:
        prevSetPoint = SendToArduino(camera) #Each loop detect and send new Setpoint
        while ser.in_waiting <= 3:
            pass
        currePosition = ReadfromArduino()
        if i == 10:
            lcd.message = "Set Point: " + str(prevSetPoint) + "\nPosition: " + str(currePosition)
            i = 0
        i = i + 1
    except IOError:
        lcd.message = "Oops! IOError, check Pi to Arduino USB Connections"

