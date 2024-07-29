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
prt = stl.comports(include_links=False)

#arduino commands
se = 'set-enable '
reset = 'reset '
sm = 'set-mode '
ss = 'set-setpoint '
st = 'status '
stop = 'stop '
logmode = 'log-mode '
prc = 'percent '
amp = 'amps '
volt = 'volts '
deg =  'degrees '


def send_command(x='COM8'):
    arduino = s.Serial(port=pn, baudrate=115200, timeout=1)
    arduino.write(bytes(x + '\n', enc))
    time.sleep(0.05)

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
    
    

    class muscle:
         def __init__(self, idnum, resistance, diameter, Length):
             self.idnum = idnum
             self.resistance = resistance
             self.diameter = diameter
             self.length = Length
          
         
         def setMode(self, cmode: str):
             mts = ''
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

    

  
