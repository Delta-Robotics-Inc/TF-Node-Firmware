import serial.tools.list_ports as stl
import serial as s
import time

devs = []
logen = " "
logout = ''
nodel =[]     
prod = '105'
pn = '' #port number
enc = 'utf-8'
arduino = serial.Serial(port=pn, baudrate=115200, timeout=1)
prt = stl.comports(include_links=False)



def send_command(x):
    arduino.write(bytes(x + '\n', enc))
    time.sleep(0.05)
       
        
class node:
    def __init__(self, idnum, port0, prodid):
        self.idnum = idnum
        self.prodid = prodid
        self.port0 = port0
    
    

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
      
     
    def getMuscle(ptn): #port number
         
        
    def setMuscle(portnum,musc):
        musc = nodes[portnum]
       
    def setLogout(path1, mode): # log path and encoding
        logout = open(path1,'w')
        if mode = 0:
            enc = "binary"
        elif mode = 1:
            enc = "utf-8"
        else:
            print("Error: Log encoding")
    
    def setLogmode(mode,bool):
        if mode = "FAST": #log mode
            
        elif mode = "SLOW":
            
        while True: #write log to file
            logout.write(' ')


def discover(proid):  #add to node list here
        ports = {}

        for por in prt:
            ports[por.pid]=por.name
        z=0
        for p in proid:
            for n in ports.keys():
                if p == n:
                    nodeob = node(z,ports[n],n)
                    nodel.append(nodeob)
                    z+=1   
              
def rediscover(idn): #id number
        nwn = idn
        ports = {}
        
        for por in prt:
            ports[por.pid]=por.name
        for n in nodel:
            if nodel[n].prodid == idn:
                nodel[n].port0 = ports[idn]       

  
while on == True:
    update
