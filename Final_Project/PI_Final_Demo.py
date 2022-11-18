# -*- coding: utf-8 -*-
"""
Spyder Editor

"""
import smbus2 #Defines I2C
import time
import SeedCV

# This is the address we setup in the Arduino Program
address = 0x04

#Write number entered to specific register
def writeBlock(value, offReg):
    bus.write_block_data(address, offReg, value)
    return -1

#Read number entered to Arduino off of previous register
def readNumber(offReg):
    number = bus.read_byte_data(address,offReg)
    return number

def stateStart():
    val = input("Begin Clyde (y/n): ")
    if val == 'y' or val == 'Y':
        writeBlock([0,0],0)
        return stateCam()
    elif val == 'n' or val == 'N':
        exit
    else:
        return stateStart()

def stateCam():
    angle, distance = cv.find_marker(markerCount)
    if angle != None:
        writeBlock([0,0],1)
        time.sleep(1)
        return stateDimensions()
    else:
        return stateCam()

def stateWait():
    status = readNumber(4)
    if status == 6:
        marker += 1
        return stateCam()
    elif status == 7:
        marker = 1
        return stateDone()
    time.sleep(.5)
    
def stateDimensions():
    angle, distance = cv.find_marker(markerCount)
    if angle != None:
        angle = int(angle*200)
        distance = int(distance*10)
        print(angle)
        print(distance)
        writeBlock([angle,distance],2)
        return stateWait()
    else:
        print("ArUco Maker Lost, Entering Search")
        writeBlock([0,0],9)
        return stateCam()

def stateDone():
    print("Finished")
    return stateStart()



bus = smbus2.SMBus(1)
time.sleep(1)

markerCount = 1
cv = SeedCV.SeedCV()

state_dictionary = {
    stateStart : "Start",
    stateCam : "Sampling Camera",
    stateWait : "Waiting for Arduino Operation",
    stateDimensions : "Sending Arduino Angle and Distance",
    stateDone : "All Done"
    }

#Until you exit loop through function
state = stateStart #Start pointer
while True:
    next_state = state()
    state = next_state


