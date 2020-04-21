import paho.mqtt.client as mqtt
import time
import json
import threading
import statistics
import matplotlib.pyplot as plt

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

ID = {"/team20/config": 0}
capLeftVals = []
capRightVals = []
sensorVals = []
PIDVals = []
statePaused = 0
stateTracking = 0

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_debug(decoded):
    if decoded["Type"] == 1:
        capLeftVals.append(decoded["Value"])
    elif decoded["Type"] == 2:
        capRightVals.append(decoded["Value"])
    elif decoded["Type"] == 3:
        sensorVals.append(decoded["Value"])
    elif decoded["Type"] == 5:
        PIDVals.append(decoded["Value"])

def on_msg_state(decoded):
    global statePaused, stateTracking
    elif decoded["Type"] == 1:
        statePaused = decoded["Value"]
    elif decoded["Type"] == 2:
        stateTracking = decoded["Value"]

def on_message(client, data, msg):
    rcvtime = round(time.time() - starttime,2)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(topic == "/team20/debug"):
        on_msg_debug(decoded)
    elif(topic == "/team20/state"):
        on_msg_state(decoded)

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
    global tests
    print("Running test: pause @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    send_config(8, 1) #enable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(3, 50)#set speed
    time.sleep(2)
    send_config(1, 2) #rover loading
    time.sleep(2)
    if statePaused == 1:
        check1 = True
    send_config(1, 1) #rover moving
    time.sleep(2)
    if statePaused == 2:
        check2 = True
    if check1 and check2:
        tests["pause"] = True

def test_PID_straight():
    print("Running test: PID straight @",round(time.time() - starttime,2))
    send_config(8, 1) #enable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 1) #enable PID
    send_config(3, 50)#set speed
    PIDVals.clear()
    time.sleep(1)
    plt.plot(PIDVals)
    plt.ylabel('PID adjustment')
    plt.xlabel('trial')
    plt.show()

def test_PID_turning():
    print("Running test: PID turning @",round(time.time() - starttime,2))
    send_config(8, 1) #enable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 1) #enable PID
    send_config(5, 50)#turn left
    PIDVals.clear()
    time.sleep(1)
    plt.plot(PIDVals)
    plt.ylabel('PID adjustment')
    plt.xlabel('trial')
    plt.show()

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
    print("distance: checking 20 inches")
    time.sleep(10)
    sensorVals.clear()
    time.sleep(1)
    if 18 < statistics.mean(sensorVals) < 22:
        check1 = True
    print("distance: checking 12 inches")    
    time.sleep(10)
    sensorVals.clear()
    time.sleep(1)
    if 10 < statistics.mean(sensorVals) < 14:
        check2 = True
    print("distance: checking 6 inches")
    time.sleep(10)
    sensorVals.clear()
    time.sleep(1)
    if 4 < statistics.mean(sensorVals) < 8:
        check3 = True
    if check1 and check2 and check3:
        tests["distance"] = True

def test_capture():
    global tests
    print("Running test: capture @",round(time.time() - starttime,2))
    check1 = False
    check2 = False
    check3 = False
    send_config(8, 1) #enable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 0) #disable PID
    send_config(3, 40) #set speed
    capLeftVals.clear()
    capRightVals.clear()
    time.sleep(5.5)
    if len(capLeftVals) >= 20 and len(capRightVals) >= 20:
        check1 = True
    if 4400 < statistics.mean(capLeftVals) < 7000:
        check2 = True
    if 4400 < statistics.mean(capRightVals) < 7000:
        check3 = True
    if check1 and check2 and check3:
        tests["capture"] = True 

def test_movement():
    global tests
    print("Running test: movement @",round(time.time() - starttime,2))
    send_config(8, 1) #enable movement
    send_config(7, 0) #disable pixy
    send_config(6, 1) #enable sensor
    send_config(2, 0) #disable PID
    time.sleep(10)
    sensorVals.clear()
    time.sleep(1)
    if 4 <= statistics.mean(sensorVal) <= 8:
        tests["movement"] = True

def test_sync():
    global tests
    print("Running test: sync @",round(time.time() - starttime,2))
    send_config(8, 0) #disable movement
    send_config(7, 0) #disable pixy
    send_config(6, 0) #disable sensor
    send_config(2, 0) #disable PID
    while(stateTracking != 0):
        pass #wait for pixy to lose target
    print("sync: target lost")
    while(stateTracking != 1):
        pass #wait for pixy to find target
    if stateTracking == 1: #found it
        tests["sync"] = True

def test_approach():
    global tests
    print("Running test: approach @",round(time.time() - starttime,2))
    send_config(8, 1) #enable movement
    send_config(7, 1) #enable pixy
    send_config(6, 1) #enable sensor
    send_config(2, 1) #enable PID
    time.sleep(5)
    sensorVals.clear()
    time.sleep(1)
    if 4 < statistics.mean(sensorVals) < 6 and stateTracking == 1:
        tests["approach"] = True

def run_tests():
    print("Thread started: run_tests")
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    run_tests = ["distance", "movement", "pause", "sync", "capture", "PID_straight", "PID_turning", "approach"]
    run_tests = ["capture"]
    for func in run_tests:
        input("Press Enter to continue to test: " + func)
        time.sleep(1)
        globals()["test_"+func]()    
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
    
