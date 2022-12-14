# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import smbus2 #Defines I2C
import time
import SeedCV

#Write number entered to specific register
def writeNumber(value, offReg):
    bus.write_block_data(address, offReg, value)
    return -1

#Read number entered to Arduino off of previous register
def readNumber(offReg):
    number = bus.read_byte_data(address,offReg)
    return number

def stateStart():
    val = input("Begin Clyde (y/n): ")
    if val == 'y' or val == 'Y':
        return stateCam()
    else:
        return stateStart()

def stateCam():
    angle, distance = cv.aruco_location()
    #angle = 2
    while angle == None:
        angle, distance = cv.aruco_location()
    if angle != None:
        writeNumber([0,0],1)
        return stateWait()
    else:
        return stateCam()

def stateWait():
    status = readNumber(4)
    time.sleep(5)
    if status == 1:
        print("Pi Waiting") #Need to change so he only waits for state done
        return stateAngle()
    #elif status == 1:
        #return stateDistance()
    elif status == 2:
        return stateDone()
    time.sleep(.5)
    #readNumber(0) Wait for Arduino to Respond with 0 to indicate stopped
    #readNumber(0) Wait for Arduino to Repond with 1 to indicate lined up
    #readNumber(0) Arduino at Correct Spot with 2 to indicate finished
    
def stateAngle():
    angle, distance = cv.aruco_location()
    print(angle)
    print(distance)
    angle = int(angle*200)
    distance = int(distance*10)
    print("PreConversion")
    print(angle)
    print(distance)
    print("Switching State")
    writeNumber([angle,distance],0)
    return stateWait
    
def stateDistance():
    angle, distance = cv.aruco_location()
    distance = int(distance*10)
    #distance = 5
    print("Pi Distance")
    writeNumber(distance, 2)
    return stateWait

def stateDone():
    print("Finished")
    return stateStart()

bus = smbus2.SMBus(1)
time.sleep(1)
# This is the address we setup in the Arduino Program
address = 0x04

state_dictionary = {
    stateStart : "Start",
    stateCam : "Sampling Camera",
    stateWait : "Arduino send Stop",
    stateAngle : "Sending Arduino Angle",
    stateDistance : "Sending Arduino Distance",
    stateDone : "All Done"
    }

cv = SeedCV.SeedCV()


#Until you exit loop through function
state = stateStart #Start pointer
while True:
    next_state = state()
    state = next_state


