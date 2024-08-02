import serial.tools.list_ports as stl
import serial as s
import time
#TODO: find parsing package
import pandas as pd
import matplotlib.pyplot as plt

#arduino commands

SE = "set-enable "
RESET = "reset "
SM = "set-mode "
SS = "set-setpoint "
ST = "status "
STOP = "stop "
LOGMODEM = "log-mode "
PRC = "percent "
AMP = "amps "
VOLT = "volts "
DEG =  "degrees "
prt = stl.comports(include_links=False)


        
              
          
devs = []
logen = " "
logout = ""
nodel =[]  # Node list 
prod = [105] # Product id list
pn = ""  # Port number
enc = "utf-8" # Encoding

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

def send_command_str(x):    
    
    arduino.write(bytes(x + "\n" , "utf-8"))
    time.sleep(0.01)
    
    print("Command sent")

def send_command(x:int , params:list):
    
    arduino.write(x, "utf-8")
    time.sleep(0.01)
    for p in params:
        arduino.write(p + "\n", "utf-8")
        time.sleep(0.01)
    

class command_t:    
    global commanddict
    commanddict = {"reset": 0xFF, "set-enable": 0x01, "set-mode": 0x02, "set-setpoint": 0x03,
               "status": 0x04, "stop": 0x05, "log-mode":0x06,
               "all":0x07, "m1":0x08, "m2": 0x09,
               "percent": 0x10, "amps":0x11, "volts": 0x12,"degree": 0x13,}   
    
    
    def reset(code = commanddict["reset"], device = "all"):
        send_command(code , [commanddict[device]])
        
    
    def set_enable(state:bool, code = commanddict["set-enable"], device = "all" ):
        
        send_command(code , [commanddict[device], state])
    
    def set_mode(mode:str, code = commanddict["set-mode"], device = "all" ):
        
        sendmode = "" #  make variables more descriptive
        if mode =="percent":
            sendmode = commanddict["percent"]
        elif mode == "amp":
            sendmode = commanddict["amps"]
        elif mode == "voltage":
            sendmode = commanddict["volts"]
        elif mode == "degree":
            sendmode = commanddict["degree"]
        else:
            print("Error: Incorrect option" )
            return        
        
        send_command(code, [commanddict[device], sendmode])
    
    def set_setpoint(mode:str, value:float, code = commanddict["set-setpoint"], device = "all" ):
        
        sendmode = "" #  make variables more descriptive
        if mode =="percent":
            sendmode = commanddict["percent"]
        elif mode == "amp":
            sendmode = commanddict["amps"]
        elif mode == "voltage":
            sendmode = commanddict["volts"]
        elif mode == "degree":
            sendmode = commanddict["degree"]
        else:
            print("Error: Incorrect option" )
            return
        
        send_command(code , [commanddict[device], sendmode, value])
    
    def status(code = commanddict["status"], device = "all"):
       
        send_command(code , [commanddict[device]])
        # TODO: recieve data from serial status
            
    def stop(code = commanddict["stop"], device = "all"):
       
        send_command(code , [commanddict[device]])
   
    def log_mode(logmode:int, code = commanddict["log-mode"], device = "all"):
        
        send_command(code, [commanddict[device], logmode])  




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
    
    def testMuscles_str(self):
        global pn
        
        pn = str(self.port0) #find variable to test in the node class
        openPort(pn)
        
        send_command_str(SS + "m1 percent 0.5")
        send_command_str(SS + "m2 percent 0.5")        
        #send_command_str("log-mode m1 0")
        
      
        send_command_str("set-enable m1 true")
        time.sleep(3.0)
       
        
        send_command_str("set-enable m1 false")
        time.sleep(3.0)
       
       
        send_command_str("set-enable m2 true")
        time.sleep(3.0)
      
        
        send_command_str("set-enable m2 false")
        time.sleep(3.0)
      
        #send_command_str("log-mode m1 0")
        
        print("Test complete")
        
        closePort(pn)
    
    def testMuscles(self):
        global pn
        
        pn = str(self.port0) #find variable to test in the node class
        openPort(pn)
        
        command_t.set_setpoint(mode = "percent", value = 0.5, device = "m1")
        command_t.set_setpoint(mode = "percent", value = 0.5, device = "m1")        
        #command_t.log_mode(logmode = 0, device = "m1")
        
      
        command_t.set_enable(state = True, device = "m1")
        time.sleep(3.0)
       
        
        command_t.set_enable(state = False, device = "m1")
        time.sleep(3.0)
       
       
        command_t.set_enable(state = True, device = "m2")
        time.sleep(3.0)
      
        
        command_t.set_enable(state = False, device = "m1")
        time.sleep(3.0)
      
        #command_t.log_mode(logmode = 0, device = "m1")
        
        print("Test complete")
        
        closePort(pn)

        
    def status(self):
        pn = str(self.port0) #find variable to test in the node class
        openPort(pn)
        command_t.status()
   
    class muscle:
         def __init__(self, idnum, resistance, diameter, Length):
             self.idnum = idnum
             self.resistance = resistance
             self.diameter = diameter
             self.length = Length
          
         
         def setMode(self, cmode: str):
             mts = "" #  make variables more descriptive
             if cmode =="percent":
                 mts = PRC
             elif cmode == "amp":
                 mts = AMP
             elif cmode == "voltage":
                 mts = VOLT
             elif cmode == "degree":
                 mts = DEG
             else:
                 print("Error: Incorrect option" )
                 return             
             
             
             send_command_str(SM + self.idnum +" "+ mts)
             
         def enable(self):
             send_command_str(SE + self.idnum + "true ")
             
         def disable(self):
             send_command_str(SE + self.idnum + "false ")
             
         def setEnable(self, bool):
             if True:
                 send_command_str(SE + self.idnum + "true ")
             elif False:
                 send_command_str(SE + self.idnum + "false ")
         
         def setSetpoint(self, cmode, spoint):
             
             send_command_str(SS + self.idnum + cmode + spoint)      
     
    def getMuscle(ptn): #port number
        pass
    
    def enableAll():
        send_command_str(SE + "all true")
    
    def disableAll():
        send_command_str(SE + "all false")
    
    def setMuscle(portnum,musc):
        pass
               
    
    def setLogging(path: str, bool, encoding = "utf-8"): #Set log path, decoding and 
        global enc
        
        
        if encoding == "binary":
            enc = "binary"
        elif encoding == "utf-8":
            enc = "utf-8"
        
        else:
            print("Error: Log encoding")
        
        while True:
            try:
                buffer = arduino.readline().decode("utf-8").strip()  # Properly decode and strip the data
                if not buffer:
                    pass  # Skip if data is empty
                
                #write to log serialinput
            except:
                KeyboardInterrupt()

discover(prod)
openPort(pn)   
node0 = nodel[0]


