import paho.mqtt.client as mqtt
import time
import json
import threading
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
         "sync": False}

topics = {"/team20/debug": ["ID", "Type", "Value"]}
test_keys = ["ID", "Type", "Value"]

ID = {"/team20/config": 0}
capLeftVals = []
capRightVals = []
sensorVal = 0
PIDBeforeVal = 0
PIDAfterVal = 0
pixyVersionVal = 0

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_debug(decoded):
    global debugval, capLeftVal, capRightVal, sensorVal, PIDBeforeVal, PIDAfterVal, pixyVersionVal
    if decoded["Type"] == 4:
        capLeftVal = decoded["Value"]
    elif decoded["Type"] == 5:
        capRightVal = decoded["Value"]
    elif decoded["Type"] == 6:
        sensorVal = decoded["Value"]
    elif decoded["Type"] == 7:
        PIDBeforeVal = decoded["Value"]
    elif decoded["Type"] == 8:
        PIDAfterVal = decoded["Value"]
    elif decoded["Type"] == 9:
        pixyVersionVal = decoded["Value"]

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
    data = {"ID": ID[topic],
            "Type": 1, #CONFIG_STATE
            "Value": 2}#ROVER_LOADING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(2)
    if capLeftVal == 0 and capRightVal == 0:
        check1 = True
    data = {"ID": ID[topic],
            "Type": 1,
            "Value": 1}#ROVER_MOVING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(2)
    if capLeftVal != 0 or capRightVal != 0:
        check2 = True
    if check1 and check2:
        tests["pause"] = True

def test_PID_straight():
    global ID, tests
    data = {"ID": ID[topic],
            "Type": 2,#PID_ENABLE
            "Value": 1}#enabled
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data = {"ID": ID[topic],
            "Type": 3,#SET_SPEED
            "Value": 50}#50/127
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1)
    x = [PIDBeforeVal]
    y = [PIDAfterVal]
##    plt.plot(x,y)
##    plt.ylabel('y')
##    plt.xlabel('x')
##    plt.show()
    time.sleep(10)
    if sensorVal == 0:
        tests["PID"] = True

def test_PID_turning():
    global ID, tests
    data = {"ID": ID[topic],
            "Type": 2,#PID_ENABLE
            "Value": 1}#enabled
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data = {"ID": ID[topic],
            "Type": 5,#TURN_LEFT
            "Value": 50}#50/127
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1)
    x = [PIDBeforeVal]
    y = [PIDAfterVal]
##    plt.plot(x,y)
##    plt.ylabel('y')
##    plt.xlabel('x')
##    plt.show()
    time.sleep(10)
    if sensorVal == 0:
        tests["PID"] = True

def test_distance():
    global tests
    check1 = False
    check2 = False
    check3 = False
    print("Running test: distance @",round(time.time() - starttime,2))
    time.sleep(10)
    if sensorVal < 22 and sensorVal > 18:
        check1 = True
    time.sleep(10)
    if sensorVal < 14 and sensorVal > 10:
        check2 = True
    time.sleep(10)
    if sensorVal < 8 and sensorVal > 4:
        check3 = True
    if check1 and check2 and check3:
        tests["distance"] = True

def test_capture():
    global tests
    print("Running test: capture @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    check3 = False
    capLeftVals = []
    capRightVals = []
    data = {"ID": ID[topic],
            "Type": 3,#SET_SPEED
            "Value": 50}#50/127
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))   
    time.sleep(5)
    if len(capLeftVals) >= 50 and len(capRightVals) >= 50:
        check1 = True
    if max(capLeftVals) < 1000 and min(capLeftVals) > 0:
        check2 = True
    if max(capRightVals) < 1000 and min(capRightVals) > 0:
        check3 = True
    if check1 and check2 and check3:
        tests["capture"] = True 

def test_movement():
    global tests
    print("Running test: movement @",round(time.time() - starttime,2))
    time.sleep(10)
    if sensorVal <= 8 and sensorVal >= 4:
        tests["movement"] = True

def test_sync():
    global tests
    print("Running test: sync @",round(time.time() - starttime,2))
    time.sleep(10)
    detected = False
    if sensorVal < 20 and detected: #send pixy offset?
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
    run_tests = ["pause"]
    for func in run_tests:
        globals()["test_"+func]()
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
    
