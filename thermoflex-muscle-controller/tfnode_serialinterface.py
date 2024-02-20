
#==============================================================================
# ThermoFlex Node Python Serial Interface
#
# This script contains the python implementation of a command line interface for the TF Node system.
#
# Putting the task of parsing strings upon the Arduino Firmware is cumbersome to the processor. All commands should follow a specified format:
# <COMMAND> <PAR1> <PAR2> <PAR3> ... <PARN>
#
# The above user-entered command would be parsed into a binary bitstream of hex data corresponding to the command and its parameters.
# Ex: "enable" hex code is 0x01.  The command "enable m1 true" would be: bitstream:[00000001 00000011 00000001] 
# -> way less data then the string version (spaces are just for show)
#
# Currently, command keys, command codes, and command params are paired in this document.  Hex codes for parameters are also specified here.
# In the future, keys and corresponding codes need to be stored in .JSON or .CSV format
#==============================================================================


import struct


def convertIntMuscle(id : int):
    return id + 0x10 #need to decide whether muscles start at "0" or "1"

#converts floating point number to hex (32 bit // 4 bytes)
def numToHex(num : float):
    return hex(struct.unpack('<I', struct.pack('<f', num))[0])

#converts integer number to hex (32 bit // 4 bytes)
def numToHex(num : int):
    return

command_codes = {
    "enable": [0x01, "muscle", "bool"], #eventually, muscle needs to be expanded into "device, muscle" to account for expanded controller network
    "set-mode": [0x02, "muscle", "ctrl-mode"],
    "set-setpoint": [0x03, "muscle", "ctrl-mode", "float"],
    "status": [0x04, "device", "muscle"],
    "stop": [0x05],
    "reset": [0xFF, "device"]
}

#define types of parameters and their possible keys.  Example: types of "muscle" parameter would include the "m1" muscle with its corresponding hex code to transmit
parameter_codes = {
    "device": {"all": 0xFF, "master": 0x00, "parse": numToHex},
    "muscle": {"all": 0xFF, "m1": 0x10, "m2": 0x11, "m3": 0x12, "parse": convertIntMuscle},
    "bool": {
        "true": 0x01, "false": 0x00, #want boolean to accept any variation of words that mean "true"
        "on": 0x01, "off": 0x02, 
        "enable": 0x01, "disable": 0x00, 
        "e": 0x01, "d": 0x00, 
        "yes": 0x01, "no": 0x00,
        "1": 0x01, "0": 0x00
    },
    "ctrl-mode": {
        "percent": 0x01, "p": 0x01, "per": 0x01, "%": 0x01, "1": 0x01,
        "volts": 0x02, "volt": 0x02, "v":0x02, "2": 0x02,
        "amps": 0x03, "amp": 0x03, "a": 0x03, "c": 0x03, "i":0x03, "3": 0x03, #every term for amps is included
        "degrees": 0x04, "degree": 0x04, "deg": 0x04, "d": 0x04, "farenheit": 0x04, "f": 0x04, "4": 0x04
    }
}


#COMMAND_KEY PARAM1 PARAM2 ... PARAMN
def sendCommand(key : str, params : list):
    pass
