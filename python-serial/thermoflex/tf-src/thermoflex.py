import serial.tools.list_ports as stl
import serial as s
import time
# find parsing 
import pandas as pd
import matplotlib.pyplot as plt
import csv
#arduino commands

SE = 'set-enable '
RESET = 'reset '       #  Make Constants all CAPS
SM = 'set-mode '
SS = 'set-setpoint '
ST = 'status '
STOP = 'stop '
LOGMODEM = 'log-mode '
PRC = 'percent '
AMP = 'amps '
VOLT = 'volts '
DEG =  'degrees '
prt = stl.comports(include_links=False)

hexlist = {"reset":{"all": ' ',"m1": ' ',"m2": ' '},
           "set-enable":{"all": {True: ' ',False: ' '},
                 "m1":  {True: ' ',False: ' '},
                     "m2":  {True: ' ',False: ' '}},
           "set-mode":{"all": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '},
               "m1": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '},
                  "m2": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '}},
           "set-setpoint":{"all": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '},
                   "m1": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '},
                      "m2": {'percent': ' ','amps': ' ', 'volts': ' ','degrees': ' '}},
           "status":{"all": ' ',"m1": ' ',"m2": ' '}, "stop":0x05,
           "log-mode":{"all":{0: " ",1: " ",2: " "},
                   "m1": {0: " ",1: " ",2: " "},
                       "m2": {0: " ",1: " ",2: " "}}
                   
           
           }

devs = []
logen = " "
logout = ''
nodel =[]  # Node list 
prod = [105] # Product id list
pn = ''  # Port number
enc = 'utf-8' # Encoding

def openPort(portnum):
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1)
    
    except:
        s.SerialException()

def closePort(portnum):
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1).close()

    except:
       s.SerialException()

def send_command(x):    
    
    arduino.write(bytes(x + '\n' , 'utf-8'))
    time.sleep(0.01)
    
    print('Command sent')
    
   

def discover(proid):  # Add to node list here
    global nodel
    
    ports = {}

    for por in prt:
        ports[por.pid]=por.name
    z=0
    for p in proid:
        for key in ports.keys():
            if p == key:
                nodeob = node(z,ports[key],key)
                nodel.append(nodeob)
                print(nodel[z].port0)
                z+=1   
                         
def rediscover(idn): #id number
   
    ports = {}
    
    for por in prt:
        ports[por.pid]=por.name
    for n in nodel:
        if nodel[n].prodid == idn:
            nodel[n].port0 = ports[idn] 
        
class node:
    def __init__(self, idnum, port0, prodid):
        self.idnum = idnum
        self.prodid = prodid
        self.port0 = port0
    
    def testMuscles(self):
        global pn
        
        pn = str(self.port0) #find variable to test in the node class
        openPort(pn)
        
        send_command(SS + 'm1 percent 0.5')
        send_command(SS + 'm2 percent 0.5')        
        #send_command("log-mode m1 0")
        
      
        send_command("set-enable m1 true")
        time.sleep(3.0)
       
        
        send_command("set-enable m1 false")
        time.sleep(3.0)
       
       
        send_command("set-enable m2 true")
        time.sleep(3.0)
      
        
        send_command("set-enable m2 false")
        time.sleep(3.0)
      
        #send_command("log-mode m1 0")
        
        print('Test complete')
        
        closePort(pn)

        
    def status(self):
        pass
   
    class muscle:
         def __init__(self, idnum, resistance, diameter, Length):
             self.idnum = idnum
             self.resistance = resistance
             self.diameter = diameter
             self.length = Length
          
         
         def setMode(self, cmode: str):
             mts = '' #  make variables more descriptive
             if cmode =='percent':
                 mts = PRC
             elif cmode == 'amp':
                 mts = AMP
             elif cmode == 'voltage':
                 mts = VOLT
             elif cmode == 'degree':
                 mts = DEG
             else:
                 print('Error: Incorrect option' )
                 return             
             
             
             send_command(SM + self.idnum +' '+ mts)
             
         def enable(self):
             send_command(SE + self.idnum + 'true ')
             
         def disable(self):
             send_command(SE + self.idnum + 'false ')
             
         def setEnable(self, bool):
             if True:
                 send_command(SE + self.idnum + 'true ')
             elif False:
                 send_command(SE + self.idnum + 'false ')
         
         def setSetpoint(self, cmode, spoint):
             
             send_command(SS + self.idnum + cmode + spoint)      
     
    def getMuscle(ptn): #port number
        pass
    
    def enableAll():
        send_command(SE + 'all true')
    
    def disableAll():
        send_command(SE + 'all false')
    
    def setMuscle(portnum,musc):
        pass
               
    
    def setLogging(path: str, bool, encoding = "utf-8"): #Set log path, decoding and 
        global enc
        
        
        if encoding == "binary":
            enc = "binary"
        elif encoding == "utf-8":
            enc = "utf-8"
        elif encoding == "ASCII":
            enc = " ascii"
        elif encoding == "unicode":
            enc = 'unicode'
        else:
            print("Error: Log encoding")
        
        while True:
            try:
                buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
                if not buffer:
                    pass  # Skip if data is empty
                
                #write to log serialinput
            except:
                KeyboardInterrupt()

discover(prod)
openPort(pn)   
node0 = nodel[0]


