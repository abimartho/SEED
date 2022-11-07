# -*- coding: utf-8 -*-
"""
Created on Tue Sep  6 15:21:35 2022

@author: Samuel Wicklund
@class: SEED Lab
"""

#The following code is a very simple state machine for checking for instances of the sequence
# "abcd". Each state function below performs an action and then implements the next state choice
# Each state returns the next state in the sequence
def state0(characterCheck):
    if characterCheck == "a":
        return stateA
    else:
        return state0

def stateA(characterCheck):
    if characterCheck == "b":
        return stateB
    elif characterCheck == "a":
        return stateA
    else:
        return state0

def stateB(characterCheck):
    if characterCheck == "c":
        return stateC
    elif characterCheck == "a":
        return stateA
    else:
        return state0

def stateC(characterCheck):
    if characterCheck == "d":
        print("abcd is contained in the string")
        return stateD
    elif characterCheck == "a":
        return stateA
    else:
        return state0

def stateD(characterCheck):
    if characterCheck == "a":
        return stateA
    else:
        return state0

# dictionary to describe the states
state_dictionary = {
    state0 : "start",
    stateA : "a",
    stateB : "b",
    stateC : "c",
    stateD : "d"
    }

#initialization
state = state0 #start pointer at state0 start
userInput = input("Input string: ")
for x in userInput: #run until last inputted character
    next_state = state(x) #launch state machine
    state = next_state #update the state machine
    
print("String check complete!") #Notify the user the function is done running
    
