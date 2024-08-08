import serial.tools.list_ports as stl
import serial as s
import time
#TODO: find parsing package
import threading as thr
#import pandas as pd
#import matplotlib.pyplot as plt

#arduino commands

SE = "set-enable "
RESET = "reset "
SM = "set-mode "
SS = "set-setpoint "
ST = "status "
STOP = "stop "
LOGMODEM = "log-mode "
PERCENT = "percent "
AMP = "amps "
VOLT = "volts "
DEG =  "degrees "
prt = stl.comports(include_links=False)


        
              
          
devs = []
logen = " "
logout = ""
logdict = {"node": [] ,
           "M1":[] ,
               "M2": []}
prod = [105] # Product id list
pn = ""  # Port number
enc = "utf-8" # Encoding


def threaded(func):
    
    def wrapper(*args, **kwargs):
        thread = thr.Thread(target=func, args=args, kwargs = kwargs)
        thread.start()
        return thread
    return wrapper



def openPort(portnum):
    "Opens a new port with given COM port"
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1)
    
    except s.SerialException:
        print('Serial not opened, check port status')

def closePort(portnum):
    "Closes the port of the given COM port"
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1).close()

    except s.SerialException:
       print('Serial not closed')

def send_command_str(x):    
    "Send the command x as a string; make sure the commands are separated by spaces(' ') "
    arduino.write(bytes(x + "\n" , "utf-8"))
    time.sleep(0.01)
    
    print("Command sent")

def send_command(x:int , params:list):
    "Sends commands recieved by command_t. Takes integer and list as arguments"
    arduino.write(x + " ", "ascii")
    time.sleep(0.01)
    for p in params:
        arduino.write(p + " ", "ascii")
        time.sleep(0.01)


        

class command_t:    
    global commanddict
    commanddict = {"reset": 0xFF, "set-enable": 0x01, "set-mode": 0x02, "set-setpoint": 0x03,
               "status": 0x04, "stop": 0x05, "log-mode":0x06,
               "all":0x07, "m1":0, "m2": 1,
               "percent": 0, "amps":1, "volts": 2,"degree": 3,"ohms": 4, "train" : 5, "manual": 6}   
    #URGENT : make alternate dictionary to work with this sending strings
    
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
        
        
        buffer = arduino.readlines(1).decode("utf-8").strip()  # Properly decode and strip the data
        if not buffer:
            print('Status not recived')
            pass  # Skip if data is empty
                
        print(buffer)
            
            
    def stop(code = commanddict["stop"], device = "all"):
       
        send_command(code , [commanddict[device]])
   
    def log_mode(logmode:int, code = commanddict["log-mode"], device = "all"):
        
        send_command(code, [commanddict[device], logmode])  



def discover(proid):  # Add to node list here
    
    global nodel
    nodel = []
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
    return nodel
                         
def rediscover(idn): #id number
   
    ports = {}
    
    for por in prt:
        ports[por.pid]=por.name
    for n in nodel:
        if nodel[n].prodid == idn:
            nodel[n].port0 = ports[idn] 
 
        
class serialMonitor:
    def __init__(self):
        pass

class node:
    def __init__(self, idnum, port0, prodid, logmode:int = 0):
        self.idnum = idnum
        self.prodid = prodid
        self.port0 = port0
        self.logmode = logmode
    
    def testMuscles_str(self): 
        
        global pn
        
        pn = str(self.port0) #find variable to test in the node class
        openPort(pn)
        
        send_command_str(SS + "m1 percent 0.5")
        send_command_str(SS + "m2 percent 0.5")        
        #send_command_str("log-mode m1 1")
        
      
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
        
        pn = str(self.port0) 
        openPort(pn)
        
        command_t.set_setpoint(mode = "percent", value = 0.5, device = "m1")
        command_t.set_setpoint(mode = "percent", value = 0.5, device = "m1")        
        #command_t.log_mode(logmode = 1, device = "m1")
        #self.logmode = 1
      
        command_t.set_enable(state = True, device = "m1")
        time.sleep(3.0)
       
        
        command_t.set_enable(state = False, device = "m1")
        time.sleep(3.0)
       
       
        command_t.set_enable(state = True, device = "m2")
        time.sleep(3.0)
      
        
        command_t.set_enable(state = False, device = "m1")
        time.sleep(3.0)
      
        #command_t.log_mode(logmode = 0, device = "m1")
        #self.logmode = 0
        print("Test complete")
        
        closePort(pn)
        
        

        
    def status(self):
        pn = str(self.port0) 
        openPort(pn)
        command_t.status(device = pn)
        
        
        for x in range(5,35): #30 lines for status check
             buffer = arduino.readline().decode("utf-8").strip()
             print(str(buffer))
   
    class muscle:
         def __init__(self, idnum, resistance, diameter, Length, mosfetn = -1):
             self.idnum = idnum #idnumbers 0 and 1
             self.resistance = resistance
             self.diameter = diameter
             self.length = Length
             self.mosfetnum = mosfetn
          
         
         def setMode(self, cmode: str):
             modesent = "" #  make variables more descriptive
             if cmode =="percent":
                 modesent = PERCENT
             elif cmode == "amp":
                 modesent = AMP
             elif cmode == "voltage":
                 modesent = VOLT
             elif cmode == "degree":
                 modesent = DEG
             else:
                 print("Error: Incorrect option" )
                 return             
             
             
             send_command_str(SM + str(self.idnum) +" "+ modesent)
             
         def enable(self):
             send_command_str(SE + str(self.idnum) + "true ")
             
         def disable(self):
             send_command_str(SE + str(self.idnum) + "false ")
             
         def setEnable(self, bool):
             if True:
                 send_command_str(SE + str(self.idnum) + "true ")
             elif False:
                 send_command_str(SE + str(self.idnum) + "false ")
         
         def setSetpoint(self, cmode, spoint):
             
             send_command_str(SS + str(self.idnum) + str(cmode) + str(spoint))      
     
    def getMuscle(self, idnum): # id number
        return self.muscle.idnum()
    
    def enableAll(self):
        send_command_str(SE + "all true")
    
    def disableAll(self):
        send_command_str(SE + "all false")
    
    def update(self):
        global reading
        reading = {"node": [] ,
                   "M1":[] ,
                       "M2": []}
        
        try:
            buffer = arduino.readline().decode("utf-8").strip()
            try:
                splitbuff = buffer.split(' ')
                splitnode = splitbuff[:splitbuff.index('M1')]
                splitm1 = splitbuff[splitbuff.index('M1') + 1:splitbuff.index('M2')]
                splitm2 = splitbuff[splitbuff.index('M2') + 2:]
            except IndexError:
                pass
            
            reading["node"] = splitnode

            reading["M1"] = splitm1

            reading["M2"] = splitm2
    
        except KeyboardInterrupt:
            False
            time.sleep(0.05)
        except s.SerialException:
            False
            time.sleep(0.05)
        
    def setMuscle(self, mosfetn:int, muscle):
        self.muscle().mosfetnum = mosfetn
               
    @threaded
    def logtoFile(self, filepath:str, state:bool = False):
          
    
        while state==True:
            try:
                buffer = arduino.readline().decode("utf-8").strip()  # Properly decode and strip the data
                if not buffer:
                    pass  # Skip if data is empty
                with open(filepath,'w') as file:
                    file.write(str(buffer))
                
            except KeyboardInterrupt:
                state == False
                
    @threaded
    def logtoPy(self, state:bool = False, printlog:bool = False, dictlog:bool = False ):
        
        usrnpt = input()
        count = 0 #counting iterations
        while state == True:
            try:
                buffer = arduino.readline().decode("utf-8").strip()  # Properly decode and strip the data
                if not buffer:
                    count += 1 #adding to the count if nothing is being sent
                    
                else:
                    count = 0 # reset the count
                   
                
                if dictlog == True:
                    try:
                        splitbuff = buffer.split(' ')
                        splitnode = splitbuff[:splitbuff.index('M1')]
                        splitm1 = splitbuff[splitbuff.index('M1') + 1:splitbuff.index('M2')]
                        splitm2 = splitbuff[splitbuff.index('M2') + 2:]
                    except IndexError:
                        pass
                    
                    for x in range(0,len(splitnode)):
                        logdict["node"][x].append(splitnode)
                    
                    for x in range(0,len(splitm1)):
                        logdict["M1"][x].append(splitm1)
                    
                    for x in range(0,len(splitm2)):
                        logdict["M2"][x].append(splitm2)
                
                if printlog == True:
                    print(str(buffer))
                
                if count>2: # count termination conditions
                    state = False
            except KeyboardInterrupt:
                state = False
                time.sleep(0.05)
                
                
            if usrnpt == 'stop':
                break
        return logdict
    # packet sending and recieving

def logging(device, mode:int):
    "Sets device logging. Takes device(all, m1,m2) as string and mode(0,1,2)"
    send_command_str("log-mode" + " " + str(device) + " " + str(mode))
def update(nood, delay:float = 1.0, stopcond = False): #choose which node to update and the delay
    
    while True:
        nood.update()
        time.sleep(delay)
        if stopcond == True:
            break
def plotting(): #pyplot plot with logdict
    pass
def endAll():
    for node in nodel:
        closePort(node.port0)

  

