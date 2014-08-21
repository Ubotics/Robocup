'''
Created on 5.June.2014

@author: Riven
'''
import web
import serial
import threading
import socket
import fcntl
import struct
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
    if len(parts)>0 and parts[1] == DEV_ULTRASONIC:
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



webtxt="""
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
#over{
    position: absolute;
    top: 271px;
    left: 195px;
}
#distance{
    position: absolute;
    top: 10px;
    left: 10px;
    font-size:25px;
    color:#ff3300
}
</style>
<script type="text/javascript">
    window.setInterval(getdistance,500)
    function getdistance()
    {
        dis = httpGet("/poll")
        console.log("distance",dis)
        document.getElementById("distance").innerHTML = dis

    }

    function httpGet(theUrl)
    {
        var xmlHttp = null;

        xmlHttp = new XMLHttpRequest();
        xmlHttp.open( "GET", theUrl, false );
        xmlHttp.send( null );
        return xmlHttp.responseText;
    } 
</script>
</head>
<body>
<div id="over">
    <img src="static/ctrl.png" usemap="#pad" width="320" height="310" border="0">
    <map name="pad">
    <area shape="rect" coords="124,41,195,113" onclick=httpGet("?move=forward") target="Message" alt="Move Forward" title="Move Forward">
    <area shape="rect" coords="38,118,117,191" onclick=httpGet("?move=left") target="Message" alt="Move Left" title="Move Left">
    <area shape="rect" coords="209,121,274,191" onclick=httpGet("?move=right") target="Message" alt="Move Right" title="Move Right">
    <area shape="rect" coords="126,196,195,268" onclick=httpGet("?move=backward") target="Message" alt="Move Backward" title="Move Backward">
    <area shape="rect" coords="128,126,196,188" onclick=httpGet("?move=stop") target="Message" alt="Stop" title="Stop">
    </map>
</div>

<div>
    <iframe src="http://HOSTIP:9000/stream_simple.html" width="100%" height="750" frameborder="no" border="0" marginwidth="0" marginheight="0" scrolling="no" allowtransparency="yes"></iframe>
</div>
<div id="distance">
@DIS
</div>

</body>
</html>
"""

urls = (
    '/', 'index',
    "/poll", "poll",
)

class index:
    def GET(self):
	global distance
        user_data = web.input()
        if not user_data.has_key("move") : 
            print
        elif user_data.move == "forward":
            Forward()
        elif user_data.move == "backward":
            Backward()
        elif user_data.move == "left":
            TurnLeft()
        elif user_data.move == "right":
            TurnRight()
        elif user_data.move == "stop":
            StopMotor()
        print distance
    	strr = webtxt.replace("@DIS","%d cm" %(distance))
	return strr
    
class poll:
    def GET(self):
        global distance
        return "%d cm" %distance


def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
		s.fileno(),
		0x8915,
		struct.pack('256s',ifname[:15])
	)[20:24])
    
localIp = get_ip_address('wlan0')
print "localip =",localIp
webtxt = webtxt.replace("HOSTIP", localIp)
ser = serial.Serial('/dev/ttyAMA0', 115200)
th = serialRead()
th.setDaemon(True)
th.start()
moveSpeed = 120
turnSpeed = 40
distance = 0
setDevice(DEV_ULTRASONIC,port3,slot1)

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
    
def StopMotor():
    dcRun(m1,0)
    time.sleep(0.2)
    dcRun(m2,0)


if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()



