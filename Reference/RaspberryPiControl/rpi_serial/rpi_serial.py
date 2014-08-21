import serial
import threading
import random
import time

port1=1
port2=2
port3=3
port4=4
port5=5
port6=6
port7=7
port8=8
m1=9
m2=10

slot1=0
slot2=1

DEV_NULL= "NULL"
DEV_ULTRASONIC= "Ultrasonic Sensor"
DEV_PIR= "PIR Motion Sensor"
DEV_BUTTON= "Button"
DEV_LIMIT= "Limit Switch"
DEV_POTENTIO= "Potentiometer"
DEV_LINEFINDER= "Line Finder"
DEV_SOUNDSENSOR= "Sound Sensor"
DEV_TEMPERATURE= "Temperature Sensor"
DEV_DCMOTOR= "DC Motor"
DEV_SERVO= "Servo Motor"
DEV_JOYSTICK= "Joystick"
DEV_LED= "Led"
DEV_IR= "Ir"
DEV_DPORT_READ= "R_DPORT"
DEV_APORT_READ= "R_APORT"
DEV_DPORT_WRITE= "W_DPORT"
DEV_APORT_WRITE="W_APORT"

class serialRead(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    
    def run(self):
        while True:
            l = ser.readline()
            #print l
            parseEcho(l)

def parseEcho(line):
    global distance
    parts = line.strip().split("/")
    if parts[1] == DEV_ULTRASONIC:
        distance = int(parts[4])

def isNumber(txt):
    try:
        int(txt,0)
        return True
    except:
        return False

def dcRun(port,value):
    cmd = "/%s/%d/%d\n" %(DEV_DCMOTOR,port,value)
    ser.write(cmd)
    print cmd
    
def servoRun(port,value):
    ser.write("/%s/%d/%d\n",DEV_SERVO,port,value)
    
def rgbRun(port,slot,red,green,blue):
    rgb565 = ((red&0x1f)<<11)+((green&0x3f)<<5)+(blue & 0x1f);
    cmd = "/%s/%d/%d/%d\n" %(DEV_LED,port,slot,rgb565)
    ser.write(cmd)
    
def setDevice(device,port,slot):
    cmd = "/%s/%d/%d\n" %(device,port,slot)
    ser.write(cmd)
    
ser = serial.Serial('/dev/ttyAMA0', 115200)
th = serialRead()
th.setDaemon(True)
th.start()
"""
while True:
    cmd = raw_input("")
    print cmd
    try:
        exec(cmd)
    except Exception,e:
        print e
"""


moveSpeed = 120
turnSpeed = 180
distance = 0
cnt=0
leftflag=False
rightflag=False

def Forward():
    dcRun(m1,moveSpeed)
    time.sleep(0.2)
    dcRun(m2,moveSpeed)

def Backward():
    dcRun(m1,-moveSpeed)
    time.sleep(0.2)
    dcRun(m2,-moveSpeed)

def TurnLeft():
    dcRun(m1,-turnSpeed)
    time.sleep(0.2)
    dcRun(m2,turnSpeed)
    
def TurnRight():
    dcRun(m1,turnSpeed)
    time.sleep(0.2)
    dcRun(m2,-turnSpeed)

setDevice(DEV_ULTRASONIC,port3,slot1)
while True:
    print cnt
    cnt=cnt+1
    if distance<40:
        randnum = random.randint(0,300)
        if randnum > 150 and rightflag==False:
            leftflag=True
            TurnLeft()
        else:
            rightflag=True
            TurnRight()
    else:
        leftflag=False
        rightflag=False
        Forward()
    time.sleep(0.2)









     
    

