import serial.tools.list_ports as stl
import serial as s
import time
#arduino commands
se = 'set-enable '
reset = 'reset '       #  Make Constants all CAPS
sm = 'set-mode '
ss = 'set-setpoint '
st = 'status '
stop = 'stop '
logmode = 'log-mode '
prc = 'percent '
amp = 'amps '
volt = 'volts '
deg =  'degrees '
prt = stl.comports(include_links=False)


devs = []
logen = " "
logout = ''
nodel =[]  # Node list 
prod = [105] # Product id list
pn = ''  # Port number
enc = 'utf-8' # Encoding

def openPort(portnum):
    global arduino
    
    arduino = s.Serial(port=portnum, baudrate=115200, timeout=1)

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
        
        send_command(ss + 'm1 percent 0.5')
        send_command(ss + 'm2 percent 0.5')        
        send_command("log-mode m1 2")
        
        for x in range(0,3):    
            send_command("set-enable m1 true")
            time.sleep(3.0)
            buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
            print(buffer)
            send_command("set-enable m1 false")
            time.sleep(3.0)
            buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
            print(buffer)
            send_command("set-enable m2 true")
            time.sleep(3.0)
            buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
            print(buffer)
            send_command("set-enable m2 false")
            time.sleep(3.0)
            buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
            print(buffer)
        
        print('Test complete')
        
        arduino.close()

        
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
                 mts = prc
             elif cmode == 'amp':
                 mts = amp
             elif cmode == 'voltage':
                 mts = volt
             elif cmode == 'degree':
                 mts = deg
             else:
                 print('Error: Incorrect option' )
                 return             
             
             
             send_command(sm + self.idnum +' '+ mts)
             
         def enable(self):
             send_command(se + self.idnum + 'true ')
             
         def disable(self):
             send_command(se + self.idnum + 'false ')
             
         def setEnable(self, bool):
             if True:
                 send_command(se + self.idnum + 'true ')
             elif False:
                 send_command(se + self.idnum + 'false ')
         
         def setSetpoint(self, cmode, spoint):
             
             send_command(ss + self.idnum + cmode + spoint)      
     
    def getMuscle(ptn): #port number
        pass
    
    def enableAll():
        send_command(se + 'all true')
    
    def disableAll():
        send_command(se + 'all false')
    
    def setMuscle(portnum,musc):
        pass
       
    
    
    def setLogout(path1, mode): # log path and encoding
        logout = open(path1,'w')
        if mode == 0:
            enc = "binary"
        elif mode == 1:
            enc = "utf-8"
        else:
            print("Error: Log encoding")
    
    def setLogmode(mode,bool):
        if mode == 0: #log mode
            pass
        elif mode == 1:
            pass
        while True: #write log to file
            logout.write(' ')

discover(prod)
try:
    openPort(pn)

except:
    s.SerialException()
    
node0 = nodel[0]

