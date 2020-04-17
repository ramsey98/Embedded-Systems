import paho.mqtt.client as mqtt
import time
import json
import threading
import statistics
#import matplotlib.pyplot as plt

connected = False
starttime = time.time()
IP = "192.168.1.45"
PORT = 1883

tests = {"pause": False,
         "PID_straight": False,
         "PID_turning": False,
         "distance": False,
         "movement": False,
         "capture": False,
         "sync": False,
         "approach": False}

topics = {"/team20/debug": ["ID", "Type", "Value"]}
test_keys = ["ID", "Type", "Value"]

ID = {"/team20/config": 0}
capLeftVals = []
capRightVals = []
sensorVal = 0
PIDBeforeVals = []
PIDAfterVals = []
stateVal = 0

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_debug(decoded):
    global debugval, sensorVal, stateVal
    if decoded["Type"] == 4:
        capLeftVals.append(decoded["Value"])
    elif decoded["Type"] == 5:
        capRightVals.append(decoded["Value"])
    elif decoded["Type"] == 6:
        sensorVal = decoded["Value"]
    elif decoded["Type"] == 7:
        PIDBeforeVals.append(decoded["Value"])
    elif decoded["Type"] == 8:
        PIDAfterVals.append(decoded["Value"])
    elif decoded["Type"] == 11:
        stateVal = decoded["Value"]

def on_message(client, data, msg):
    global pub_results, tests, reset, started 
    rcvtime = round(time.time() - starttime,2)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
        if(topic == "/team20/debug"):
            on_msg_debug(decoded)
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def send_config(msgType, msgValue):
    global ID
    topic = "/team20/config"
    data = {"ID": ID[topic],
            "Type": msgType,
            "Value": msgValue}
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    
def test_pause():
    global ID, tests, capLeftVal
    print("Running test: pause @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    topic = "/team20/config"
    capLeftVal = 0
    while(capLeftVal == 0):
        time.sleep(.1) #wait until moving
    print("Pause: detected movement")
    send_config(1, 2) #rover loading
    time.sleep(2)
    if capLeftVal == 0 and capRightVal == 0:
        check1 = True
    send_config(1, 1) #rover moving
    time.sleep(2)
    if capLeftVal != 0 or capRightVal != 0:
        check2 = True
    if check1 and check2:
        tests["pause"] = True

def test_PID_straight():
    global ID, tests
    print("Running test: PID straight @",round(time.time() - starttime,2))
    send_config(2, 1)#enable PID
    send_config(3, 50)#set speed
    PIDBeforeVals = []
    PIDAfterVals = []
    time.sleep(1)
##    plt.plot(PIDBeforeVals,PIDAfterVals)
##    plt.ylabel('y')
##    plt.xlabel('x')
##    plt.show()

def test_PID_turning():
    global ID, tests
    print("Running test: PID turning @",round(time.time() - starttime,2))
    send_config(2, 1)#enable PID
    send_config(5, 50)#turn left
    PIDBeforeVals = []
    PIDAfterVals = []
    time.sleep(1)
##    plt.plot(PIDBeforeVals,PIDAfterVals)
##    plt.ylabel('y')
##    plt.xlabel('x')
##    plt.show()

def test_distance():
    global tests
    print("Running test: distance @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    check3 = False
    send_config(8, 0) #disable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 0) #disable PID
    time.sleep(10)
    if 18 < sensorVal < 22:
        check1 = True
    time.sleep(10)
    if 10 < sensorVal < 14:
        check2 = True
    time.sleep(10)
    if 4 < sensorVal < 8:
        check3 = True
    if check1 and check2 and check3:
        tests["distance"] = True

def test_capture():
    global tests
    print("Running test: capture @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    check3 = False
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 0) #disable PID
    send_config(3, 50) #set speed
    capLeftVals.clear()
    capRightVals.clear()
    time.sleep(5)
    if len(capLeftVals) >= 25 and len(capRightVals) >= 25:
        check1 = True
    if statistics.mean(capLeftVals) < 7000 and statistics.mean(capLeftVals) > 4400:
        check2 = True
    if statistics.mean(capRightVals) < 7000 and statistics.mean(capRightVals) > 4400:
        check3 = True
    if check1 and check2 and check3:
        tests["capture"] = True 

def test_movement():
    global tests
    print("Running test: movement @",round(time.time() - starttime,2))
    send_config(7, 0) #disable pixy
    time.sleep(10)
    if sensorVal <= 8 and sensorVal >= 4:
        tests["movement"] = True

def test_sync():
    global tests
    print("Running test: sync @",round(time.time() - starttime,2))
    while(stateVal != 26):
        pass #wait for pixy to lose poster
    time.sleep(10)
    if stateVal == 27: #27 == PIXY_TRACKING
        tests["sync"] = True

def test_approach():
    global tests
    print("Running test: approach @",round(time.time() - starttime,2))
    time.sleep(20)
    if 4 < sensorVal < 6 and stateVal == 27: #27 == PIXY_TRACKING
        tests["sync"] = True

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    run_tests = ["distance", "movement", "pause", "sync", "capture", "PID_straight", "PID_turning"]
    run_tests = ["distance", "movement"]
    for func in run_tests:
        globals()["test_"+func]()
        input("Press Enter to continue to test: " + func)
        time.sleep(delay)    
    print("Completed Tests @",round(time.time() - starttime,2))
    print(tests)
       
def main():
    global client
    print("Thread started: main")
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message
    client.connect(IP,PORT,60)
    client.loop_forever()
    
if __name__ == "__main__":
    threading.Thread(target=main).start()
    threading.Thread(target=run_tests).start()    
    
