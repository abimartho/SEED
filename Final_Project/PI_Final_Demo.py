# -*- coding: utf-8 -*-
"""
Spyder Editor

"""
import smbus2 #Defines I2C
import time
import SeedCV



#Write number entered to specific register
def writeBlock(value, offReg):
    bus.write_block_data(address, offReg, value)
    return -1

#Read number entered to Arduino off of previous register
def readNumber(offReg):
    number = bus.read_byte_data(address,offReg)
    return number

def incMarker(markerCount):
    markerCount += 1

def stateStart(markerCount):
    val = input("Begin Clyde (y/n): ")
    if val == 'y' or val == 'Y':
        writeBlock([0,0],0)
        return stateCam(markerCount)
    elif val == 'n' or val == 'N':
        exit
    else:
        return stateStart(markerCount)

def stateCam(markerCount):
    angle, distance = cv.find_marker(markerCount)
    if angle != None:
        writeBlock([0,0],1)
        time.sleep(1)
        return stateDimensions(markerCount)
    else:
        return stateCam(markerCount)

def stateWait(markerCount):
    time.sleep(1)
    status = readNumber(4)
    if status == 6:
        markerCount += 1
        return stateCam(markerCount)
    elif status == 7:
        markerCount = 1
        return stateDone(markerCount)
    else:
        return stateWait(markerCount)
    
def stateDimensions(markerCount):
    angle, distance = cv.find_marker(markerCount)
    if angle != None:
        angle = int(angle*200)
        distance = int(distance*10)
        print(angle)
        print(distance)
        writeBlock([angle,distance],2)
        return stateWait(markerCount)
    else:
        print("ArUco Maker Lost, Entering Search")
        writeBlock([0,0],9)
        return stateCam(markerCount)

def stateDone(markerCount):
    print("Finished")
    return stateStart(markerCount)

# This is the address we setup in the Arduino Program
address = 0x04

bus = smbus2.SMBus(1)
time.sleep(1)


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
markerCount = 1
while True:
    next_state = state(markerCount)
    state = next_state


