import serial.tools.list_ports as stl
import serial as s
import time
import threading as thr


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

logdict = {"node": [] ,
           "M1":[] ,
               "M2": []}
prod = [105] # Product id list


def threaded(func):
    global threadlist
    threadlist = []
    
    def wrapper(*args, **kwargs):
        thread = thr.Thread(target=func, args=args, kwargs = kwargs)
        thread.start()
        threadlist.append(thread)
        return thread

    return wrapper
   


def send_command_str(node:object, command):
    '''
    
    Send the command x as a string; Takes command_t object as arguments.
    
    '''
    port = node.openPort() 
    if command.code == 0xFF or 0x04:
        port.write(command.name + " ", "utf-8")
        time.sleep(0.01)
        port.write(command.device, "utf-8")
        
    else:    
        port.write(command.name + " ", "utf-8")
        time.sleep(0.01)
        
        port.write(command.device + " ", "utf-8")
        time.sleep(0.01)
        
        for p in command.params:
            port.write(str(p) + " ", "utf-8")  # Current string implementation
            time.sleep(0.01)
            
    time.sleep(0.05)

def send_command(node:object, command):
    '''
    
    Sends commands recieved by command_t. Takes command_t object as arguments.
    
    '''
    
    port = node.openPort()
    
    if command.code == 0xFF or 0x04:
        port.write(command.code, "ascii") #TODO: check if send
        time.sleep(0.01)
        
        port.write(command_t.devicedef[command.device], "ascii")
        time.sleep(0.01)
        
    else:    
        port.write(command.code, "ascii")
        time.sleep(0.01)
        
        port.write(command_t.devicedef[command.device], "ascii")
        time.sleep(0.01)
        for p in command.params:
            port.write(p, "ascii")  # Current string implementation
            time.sleep(0.01)
    time.sleep(0.05)      

class command_t:
    
    '''
    
    Class dedicated to holding and sending the command codes.
    
    '''
    
    commanddefs = {"set-enable": [0x01, [bool]], #state
			       "set-mode": [0x02, [int]], #mode
			       "set-setpoint": [0x03, [int, float]], #mode, value
			       "status": [0x04, []],
			       "log-mode": [0x06, [int]], #log mode(subject to change)
			       "reset": [0xFF, []] 
			       }
	
    devicedef = {"all": 0x07, "node": 0x00, "m1": 0x01, "m2": 0x02}
	
    modedef = {"percent": 0, "amps": 1, "volts": 2,"degree": 3,"ohms": 4, "train" : 5, "manual": 6}
          
        
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
        

    def __init__(self, name:str, params : list, device = 'all' ):
    	
        self.params = params 
        self.device = device
        self.name = name
        try:
            self.code = command_t.commanddefs[name]
        except:
            self.code = 0x00  # Invalid code reserved for 0
            raise KeyError
            print("Invalid name")
            
                
                       
        if command_t.isValid(self.name, params) is True:
            pass
        elif command_t.isValid(self.name, params) is False:
           raise ValueError()
           print("Incorrect arguments for this command")
            
           
        


def discover(proid = prod):  # Add to node list here
    '''
   
    Takes a list of product id's and returns a list of Node-class objects.
    
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
                nodel[z].openPort(nodel[z].port0)
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
    def __init__(self, idnum, port0, prodid, logmode:int = 0, mosports:int = 2, command_buff = []):
        self.idnum = idnum
        self.prodid = prodid
        self.port0 = port0
        self.logmode = logmode
        self.mosports = mosports
        self.muscles = {}
        self.command_buff = []
        self.logbuffer = ""
     
        
    def openPort(self): #TODO: make port dictionary to track node and port combinations
        '''
        
        Opens a new port with given COM port. Returns serial port.
        
        '''
        
        try:
            arduino = s.Serial(port = self.port0 , baudrate=115200, timeout=1)
        
        except s.SerialException:
            print('Serial not opened, check port status')
            
            return arduino

    def closePort(self):
        '''
        
        Closes the port of the given COM port.
        
        '''
        
        
        try:
            s.Serial(port=self.port0, baudrate=115200, timeout=1).close()

        except s.SerialException:
           print('Serial not closed')
           
    def testMuscles(self, sendformat:int = 1): #TODO: fix this/split from string vs. normal
        '''
        
        Tests the node and muscle connections. Send format takes integer; 0 for ascii, 1 for string format

        '''       
        
        pn = str(self.port0) 
        self.openPort(pn)
        mode = PERCENT
        if sendformat == 1:
            send_command_str(command_t("set-setpoint", [mode ,0.5], device = "m1")) # make own test command
            send_command_str(command_t("set-setpoint", [mode ,0.5], device = "m2"))        
            #send_command_str(command_t("logmode", [1],device = "all"))
            #self.logmode = 1
          
            send_command_str(command_t("set-enable", [True], device = "m1"))
            time.sleep(3.0)
           
            
            send_command_str(command_t("set-enable", [False], device = "m1"))
            time.sleep(3.0)
           
           
            send_command_str(command_t("set-enable", [True], device = "m2"))
            time.sleep(3.0)
          
            
            send_command_str(command_t("set-enable", [False], device = "m2"))
            time.sleep(3.0)
          
            #send_command_str(command_t("logmode", [0],device = "all"))
            #self.logmode = 0
            print("Test complete")
        elif sendformat == 0:
            
            send_command(command_t("set-setpoint", [mode ,0.5], device = "m1")) # make own test command
            send_command(command_t("set-setpoint", [mode ,0.5], device = "m2"))        
            #send_command(command_t("logmode", [1],device = "all"))
            #self.logmode = 1
          
            send_command(command_t("set-enable", [True], device = "m1"))
            time.sleep(3.0)
           
            
            send_command(command_t("set-enable", [False], device = "m1"))
            time.sleep(3.0)
           
           
            send_command(command_t("set-enable", [True], device = "m2"))
            time.sleep(3.0)
          
            
            send_command(command_t("set-enable", [False], device = "m2"))
            time.sleep(3.0)
          
            #send_command(command_t("logmode", [0],device = "all"))
            #self.logmode = 0
            print("Test complete")
        
        self.closePort(pn)
        
        
    def status(self):
        '''
        
        Requsts and collects the status from the device.
                
        '''
        
        self.openPort()
        status = command_t(name = 'status', params = [0])
        send_command(status)
        
        #TODO: track changes to serial line
        #while True:
        for x in range(0,30): #30 lines for status check
            buffer = self.openPort().readline().decode("utf-8").strip()
            #if buffer == 
                                
            print(str(buffer))
    
        
    def setMode(self, conmode: str, device = 'all'):
        '''
        
        Sets the data mode that the muscle will recieve.
        
        '''
        cmode = None
        if conmode =="percent":
            cmode = PERCENT
        elif conmode == "amp":
            cmode = AMP
        elif conmode == "voltage":
            cmode = VOLT
        elif conmode == "degree":
            cmode = DEG
        else:
            print("Error: Incorrect option" )
            return
        
           
        
        if device == "all":
            for m in self.muscles.values():
                m.setMode(cmode)
        else:
            for m in self.muscles.keys():
                if device == m:
                    self.muscles[m].setMode(cmode)
                
                
    def setSetpoint(self, musc:int, cmode, setpoint:int):   #takes muscle object idnumber and 
        
        muscl = "m" + str(self.muscles[musc].mosfetnum)       
        
        command = command_t(name = "set-setpoint", device = muscl, params = [cmode, setpoint])
        
        self.command_buff.append(command)      
     
    def setMuscle(self, idnum:int, muscle:object): # takes muscle object and idnumber and adds to a dictionary
        '''
        
        Adds the selected muscle to the node and assigns an id number
        
        '''
        self.muscles[idnum] = muscle
        muscle.masternode = self.idnum
    
    def enableAll(self):
        '''
        
        Enables all muscles.
        
        '''
        
        for x in self.muscles.keys():
            command = command_t(name  = "set-enable", params = [True] ) 
            self.command_buff.append(command)
    
    def disableAll(self):
        '''
        
        Disables all muscles.
        
        '''
        for x in self.muscles.keys():
            command = command_t(name  = "set-enable", params = [False] )
            self.command_buff.append(command)
    
    def update(self, sendformat:int = 0):
        '''
        
        Updates the status of the node. Send format is by default 0-ascii or 1-string format.
        
        '''
        for x in self.command_buff:
            try:
                self.buffer = self.openPort().readline().decode("utf-8").strip()
            
            finally: 
            
                if sendformat == 0:
               
                    send_command(self,x)
                    self.command_buff.remove(x)
            
                elif sendformat == 1:        
                
                    send_command_str(self,x)
                    self.command_buff.remove(x)
                
                else:
                    raise ValueError
            
   
               
    @threaded
    def logtoFile(self, filepath:str, state:bool = False):
        '''
        
        Writes log data to a file.
        

        '''
        while state==True:
            try:
                self.buffer  # Properly decode and strip the data
                if not self.buffer:
                    pass  # Skip if data is empty
                with open(filepath,'w') as file:
                    file.write(str(self.buffer))
                
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
                self.buffer  # Properly decode and strip the data
                if not self.buffer:
                    count += 1 #adding to the count if nothing is being sent
                    
                else:
                    count = 0 # reset the count
                   
                
                if dictlog == True:
                    try:
                        splitbuff = self.buffer.split(' ')
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
                    print(str(self.buffer))
                
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
    def __init__(self, idnum:int, resist, diam, length, masternode:object = None):
         self.idnum = idnum 
         self.resistance = resist
         self.diameter = diam
         self.length = length
         self.cmode = PERCENT
         self.mosfetnum = None
         self.masternode = masternode
      
    def setMusclemos(self, mosfetnum:int):
        '''
        
        Assigns muscle to a mosfet number
        
        '''
        self.mosfetnum = mosfetnum 
    
    def setMode(self, conmode: str):
         '''
         
         Sets the data mode that the muscle will recieve.
         
         '''
         
         if conmode =="percent":
             self.cmode = PERCENT
         elif conmode == "amp":
             self.cmode = AMP
         elif conmode == "voltage":
             self.cmode = VOLT
         elif conmode == "degree":
             self.cmode = DEG
         else:
             print("Error: Incorrect option" )
             return             
         
         
         self.masternode.command_buff.append(SM + str('m' + self.idnum+1 + ' ') +" "+ self.cmode)
         
    def enable(self):
        '''
        
        Enables the muscle selected.
        
        '''
        self.masternode.command_buff.append(SE + str('m' + self.idnum+1 + ' ') + "true ")
 
    def disable(self):
        '''
        
        Disables the muscle selected.
        
        '''
        self.masternode.command_buff.append(SE + str('m' + self.idnum+1 + ' ') + "false ")
 
    def setEnable(self, bool):
        '''
        Sets the enable staus of the muscle.
        
        Parameters
        ----------
        bool : TYPE
       
    
        '''
        if True:
            self.masternode.command_buff.append(SE + str('m' + self.idnum+1 + ' ') + "true ")
        elif False:
            self.masternode.command_buff.append(SE + str('m' + self.idnum+1 + ' ') + "false ")
         
def logging(device, mode:int):
    '''
    
    Sets device logging. Takes device(all, m1,m2) as string and mode(0,1,2)
    
    '''
    send_command_str("log-mode" + " " + str(device) + " " + str(mode))

def update(): #choose which node to update and the delay
    '''
    
    updates all nodes to send commands and recieves all data
    
    '''
         
    for node in nodel:
        node.update()
        
def delay():
    
    for node in nodel:
        node.buffer = node.openPort().readline().decode("utf-8").strip()
        
    
def plotting(): #pyplot plot with logdict
    pass
def endAll():
    '''
    
    Closes all node ports. and end all threads.
    
    '''
    for t in threadlist:
        t.join()
    for node in nodel:
        node.closePort()

  #TODO: make the command_t commands and notate so that they can be easily implimented
  #TODO: make markdown file with intrstions on how to use code and package

