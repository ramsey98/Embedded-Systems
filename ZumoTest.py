import paho.mqtt.client as mqtt
import time
import json
import threading

connected = False
starttime = time.time()
IP = "192.168.1.45"
PORT = 1883

tests = {"pause": False,
         "PID": False,
         "distance": False,
         "movement": False}

topics = {"/team20/debug": ["ID", "value"]}
test_keys = ["ID", "Type", "Value"]

ID = {"/team20/config": 0}
debugval = 0

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_debug(decoded):
    global debugval
    debugval = decoded["value"]

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
    global ID, tests
    print("Running test: pause @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {"ID": ID[topic],
            "Type": 1, #CONFIG_STATE
            "value": 2}#ROVER_LOADING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(10) #should be paused
    if debugval == 0: #change this to output speed?
        tests["pause"] = True
    data = {"ID": ID[topic],
            "Type": 1,
            "Value": 1}#ROVER_MOVING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_PID():
    global ID, tests
    print("Running test: PID @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {"ID": ID[topic],
            "Type": 2, #CONFIG_PID_KP
            "value": 0}
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))    
    data = {"ID": ID[topic],
            "Type": 3, #CONFIG_PID_KI
            "Value": 0}#
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(10)
    if debugval == 0:
        tests["PID"] = True

def test_distance():
    global tests
    check1 = False
    check2 = False
    check3 = False
    print("Running test: distance @",round(time.time() - starttime,2))
    time.sleep(10)
    if debugval < 22 and debugval > 18:
        check1 = True
    time.sleep(10)
    if debugval < 14 and debugval > 10:
        check2 = True
    time.sleep(10)
    if debugval < 8 and debugval > 4:
        check3 = True
    if check1 and check2 and check3:
        tests["distance"] = True

def test_movement():
    global tests
    print("Running test: movement @",round(time.time() - starttime,2))
    time.sleep(10)
    if debugval < 8 and debugval > 4:
        tests["movement"] = True       

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    #run_tests = ["config", "badPayload", "noID", "repeatID", "skipID", "overflowBuf", "time", "receive", "send", "reconnect", "dos"]
    run_tests = ["pause", "PID"]
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
    
