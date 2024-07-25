import serial as s
import time as time
import

devs = []
logen = " "
logout = 
     


class node:
    def __init__(self,idnum):
        self.idnum = idnum
    
    def discover():
    devs = s.tools.list_ports.comports(include_links=True)
    cnt = 1
    for dev in devs:
         node(cnt) = dev
         cnt+=1
         #add to node list here
    
    def rediscover(node):
    
    def getMuscle():
        
        
    def setMuscle(portnum,musc):
        musc = nodes[portnum]
       
    def setLogout(path1, mode): # log path and encoding
        logout = open(path1,'w')
        if mode = 0:
            logen = "binary"
        elif mode = 1:
            logen = "UTF_8"
        else:
            print("Error: Log encoding")
    
    def setLogmode(mode,bool):
        if mode = "FAST": #log mode
            
        elif mode = "SLOW":
            
        while True: #write log to file
            logout.write(' ')
            
            
            
class muscle:
     def __init__(self, resist: resistance, diam: diameter, length: Length):
         self.resistance = resistance
         self.diameter = diameter
         self.length = Length
     
     def setMode(controlMode: mode):
         if mode=0:
             
         
     def enable():
         
     def disable():
         
     def setEnable(bool):
     
     def setSetpoint(controlMode: mode,setpoint: point):


