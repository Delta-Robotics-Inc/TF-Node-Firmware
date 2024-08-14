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
    '''
    
    Opens a new port with given COM port.
    
    '''
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1)
    
    except s.SerialException:
        print('Serial not opened, check port status')

def closePort(portnum):
    '''
    
    Closes the port of the given COM port.
    
    '''
    
    global arduino
    
    try:
        arduino = s.Serial(port=portnum, baudrate=115200, timeout=1).close()

    except s.SerialException:
       print('Serial not closed')

def send_command_str(x):    
    '''
    
    Send the command x as a string; make sure the commands are separated by spaces(' ') .
    
    '''
    arduino.write(bytes(x + "\n" , "utf-8"))
    time.sleep(0.01)
    
    update_queue(x)

def send_command(command):
    '''
    
    Sends commands recieved by command_t. Takes integer and list as arguments.
    
    '''
    
    arduino.write(command.name + " ", "ascii")
   
    time.sleep(0.01)
    for p in command.params:
        arduino.write(p + " ", "ascii")  # Current string implementation
        #arduino.write(p, hex)  # Code will be changed to this
        time.sleep(0.01)

def send_update_command(x):    
    '''
    
    Send the command x as a string; make sure the commands are separated by spaces(' ') .
    
    '''
    arduino.write(bytes(x + "\n" , "utf-8"))
    time.sleep(0.01)
    
    
def update_queue(command):
    global command_queue
    
    command_queue = []
    
    
    if len(command_queue) > 6:
        command_queue.append(command)
        command_queue.pop(0)
    else:
        command_queue.append(command)
        
        

class command_t:
    
    '''
    
    Class dedicated to holding and sending the command codes.
    
    '''
    
    commanddefs = {"set-enable": [0x01, [int, bool]],
			       "set-mode": [0x02, [int, int]],
			       "set-setpoint": [0x03, [int, int, float]],
			       "status": [0x04, [int]],
			       "log-mode": [0x06, [int, int]],
			       "reset": [0xFF, [int]]
			       }
	
    device = {"all": 0x07, "node": 0x00, "m1": 0x01, "m2": 0x02}
	
    mode = {"percent": 0, "amps":1, "volts": 2,"degree": 3,"ohms": 4, "train" : 5, "manual": 6}
          
        
    def getName(code:hex):
       for x in command_t.commanddefs:
           if code == command_t.commanddefs[x][0]:
               return x

    def isValid(self, command, params: str):
       ''' 
           
       Check if name, code, and params match one of the definitions.
           
       '''
       z = 0 
       for x in params:
           if type(x) == self.commanddefs[command][1][z]:
               z+=1
               continue
           else:
               return False
       return True
        

    def __init__(self, params : list, name = 0, code = 0, ):
    	
        self.params = params 
        
        if type(name) is str:
            self.name = name
        else:
            try:
                self.name = command_t.getName(code)  # Need to implement function to iterate through commanddefs and find the correct name, return "none" if none found
            except TypeError:
                print('Name not found')
        if type(code) == hex:
            try:
                self.code = command_t.commanddefs[name]
            except KeyError:
                self.code = 0x00  # Invalid code reserved for 0
                
                       
        if command_t.isValid(self.name, params) is True:
            pass
        elif command_t.isValid(self.name, params) is False:
            print('Parameters incorrectly formatted')
            
           
        


def discover(proid):  # Add to node list here
    '''
   
    Takes a list of product id's and creates a list of Node-class objects.
    
    Parameters
    ---------
    proid : list
        DESCRIPTION. A list of int values that correspond with the producti id
   
    RETURNS
    ----------
    nodel: list
        DESCRIPTION. A list of the node objects with their idnumbers, ports, and product id as identifiers
    '''
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
    '''
    
    Takes node-object idnumber and tries to find corresponding port.
    
    '''
    ports = {}
    
    for por in prt:
        ports[por.pid]=por.name
    for n in nodel:
        if nodel[n].prodid == idn:
            nodel[n].port0 = ports[idn] 
 
    #TODO Later: use serial numbers to track individual devices
        

class node:
    def __init__(self, idnum, port0, prodid, logmode:int = 0, mosports:int = 2):
        self.idnum = idnum
        self.prodid = prodid
        self.port0 = port0
        self.logmode = logmode
        self.mosports = mosports
    
    command_buff = []
        
    
    def testMuscles_str(self):
        '''
        
        Tests the node and muscle connections.

        '''
        
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
        '''
        
        Tests the node and muscle connections.

        '''       
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
        '''
        
        Requsts and collects the status from the device.
                
        '''
        pn = str(self.port0) 
        openPort(pn)
        send_command_str('status all')
        #status = command_t(name = 'status', code = 0x04, params = [0])
        #send_command(status)
        
        
        for x in range(0,30): #30 lines for status check
             buffer = arduino.readline().decode("utf-8").strip()
             print(str(buffer))
    
    def command():
        pass
        
    def setSetpoint(self, cmode, spoint):
             
        send_command_str(SS + str(self.idnum) + str(cmode) + str(spoint))      
     
    def addMuscle(muscle,idnum): # id number
        '''
        
        Adds the selected muscle to the node and assigns an id number
        
        '''
       
    
    def enableAll(self):
        '''
        
        Enables all muscles.
        
        '''
        send_command_str(SE + "all true")
    
    def disableAll(self):
        '''
        
        Disables all muscles.
        
        '''
        send_command_str(SE + "all false")
    
    def update(self):
        '''
        
        Updates the status of the node.
        
        '''
        
    
        for x in self.command_buff:
            send_command(x)
        
   
               
    @threaded
    def logtoFile(self, filepath:str, state:bool = False):
        '''
        
        Writes log data to a file.
        

        '''
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
        '''
        
        Sends log data to terminal output or dictionary
        
        '''
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
class muscle:
    def __init__(self, idnum, resistance, diameter, Length, mosfetn = -1):
         self.idnum = idnum #idnumbers 0 and 1
         self.resistance = resistance
         self.diameter = diameter
         self.length = Length
         self.mosfetnum = mosfetn
      
    def setMuscle(self, mosfetnum):
        '''
        
        Assigns muscle to a mosfet number
        
        '''
        self.mosfetnum = mosfetnum 
    
    def setMode(self, cmode: str):
         '''
         
         Sets the data mode that the muscle will recieve.
         
         '''
         modesent = "" 
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
        '''
        
        Enables the muscle selected.
        
        '''
        send_command_str(SE + str(self.idnum) + "true ")
 
    def disable(self):
        '''
        
        Disables the muscle selected.
        
        '''
        send_command_str(SE + str(self.idnum) + "false ")
 
    def setEnable(self, bool):
        '''
        Sets the enable staus of the muscle.
        
        Parameters
        ----------
        bool : TYPE
       
    
        '''
        if True:
            send_command_str(SE + str(self.idnum) + "true ")
        elif False:
            send_command_str(SE + str(self.idnum) + "false ")
         
def logging(device, mode:int):
    '''
    
    Sets device logging. Takes device(all, m1,m2) as string and mode(0,1,2)
    
    '''
    send_command_str("log-mode" + " " + str(device) + " " + str(mode))

def update(): #choose which node to update and the delay
    '''
    
    updates all nodes to send commands and recieves all data
    
    '''
    #TODO: create a command queue and update muscles via command queue
    global reading
    reading = {"node": [] ,
               "M1":[] ,
                   "M2": []}
    #TODO: modify this so it takes data from multiple nodes
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
    
        for node in nodel:
            node.update()
    
    except KeyboardInterrupt:
        False
        time.sleep(0.05)
    except s.SerialException:
        False
        time.sleep(0.05)    
    
def plotting(): #pyplot plot with logdict
    pass
def endAll():
    '''
    
    Closes all node ports.
    
    '''
    for node in nodel:
        closePort(node.port0)

  #TODO: make the command_t commands and notate so that they can be easily implimented
  #TODO: make markdown file with intrstions on how to use code and package

