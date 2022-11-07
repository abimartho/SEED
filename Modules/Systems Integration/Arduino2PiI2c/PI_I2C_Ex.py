# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import smbus2 #Defines I2C
import time

def stateCam():
    angle = compVis.aruco_location(camera,dimension)
    if angle != None:
        #writeNumber('1',0) write Stop to register 0
        return stateWait(angle)
    else:
        return stateCam

def stateWait():
    status = readNumber(0)
    if status == 0:
        return stateAngle()
    elif status == 1:
        return stateDistance()
    elif status == 2:
        return stateDone()
    #readNumber(0) Wait for Arduino to Respond with 0 to indicate stopped
    #readNumber(0) Wait for Arduino to Repond with 1 to indicate lined up
    #readNumber(0) Arduino at Correct Spot with 2 to indicate finished
    
def stateAngle():
    angle = compVis.aruco_location(camera,dimension)
    writeNumber(angle, 1)
    return stateWait
    
def stateDistance():
    return stateWait

def stateDone():
    print("Finished")

bus = smbus2.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

state_dictionary = {
    stateCam : "Sampling Camera",
    stateWait : "Arduino send Stop",
    stateAngle : "Sending Arduino Angle",
    stateDistance : "Sending Arduino Distance",
    stateDone : "All Done"
    }



#Write number entered to specific register
def writeNumber(value, offReg):
    bus.write_byte_data(address, offReg, value)
    return -1

#Read number entered to Arduino off of previous register
def readNumber(offReg):
    number = bus.read_byte_data(address,offReg)
    return number

#Until you exit loop through function
state = stateCam #Start pointer
while True:
    next_state = state()
    state = next_state
    
 #Below is code that is best used as an example for I2C to Pi, due to how the code works its best to keep below as an example   
    #var = input('Enter 1 – 9: ') #Example input
    #var = int(var)
    #offset = input ('Enter 0 or 1: ') #Example "Register"
    #offset = int(offset)
    #while (offset != 1) and (offset != 0): #Catch non 0 or 1 input
        #print("Input must be 0 or 1!")
        #offset = input ('Enter 0 or 1: ')
        #offset = int(offset)
    #if not var:
        #continue
    #writeNumber(var,offset) #Write to Arduino Using Offset and Var
    #print('RPI: Hi Arduino, I sent you ', var)
    # sleep one second
    #time.sleep(1)
    
    #number = readNumber(offset) #Read from Arduino at current register
    #print('Arduino: Hey RPI, I received and modified a digit ', number)
    #print()

