import paho.mqtt.client as mqtt
import time
import json
import threading

connected = False
starttime = time.time()
IP = "192.168.1.45"
PORT = 1883

##tests = {"size": False, #size of one payload > 256
##         "repeatID": False, #send consecutive jsons of repeat ID to board
##         "skipID": False, #send consecutive jsons skipping several IDs to board
##         "noID": False, #send empty json with no ID to board
##         "badPayload": False, #send json with invalid entries to board
##         "reconnect": False, #disconnect and reconnect board
##         "json": False, #verify msgs from board are json format
##         "stats": False, #verify board is sending stats msgs
##         "config": False, #send configuration value to board on config topic, board returns value on debug topic
##         "overflow": False, #send json to board exceding data buffer (512)
##         "time": False, #verify >10 msgs/sec
##         "receive": False, #verify board receives all of a series of msgs
##         "send": False, #verify all board messages are getting sent
##         "dos": False} #flood board with msgs, causing it to halt

tests = {"pause": False}

topics = {"/team20/stats": ["ID", "Attempts", "Received", "Missed"],
          "/team20/errors": ["ID", "Type"],
          "/team20/debug": ["ID", "value"]}
test_keys = ["ID", "value"]

pub_results = {topic: {"Successes": 0, "Failures": 0, "Time": 0} for topic in topics.keys()}
pub_stats = {"Attempts": 0,
             "Received": 0,
             "Missed": 0}
ID = {topic: False for topic in topics.keys()}
ID["/team20/config"] = 0
reset = False
started = {topic: 0 for topic in topics.keys()}
debugval = 0
errors = [0]
badpayloadval = 1
overflowval = 2
skipIDval = 3
repeatIDval = 4
noIDval = 5
dosval = 6

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_stats(decoded):
    global pub_stats, tests
    if(set(topics["/team20/stats"]).issubset(set(decoded.keys()))):
        pub_stats["Attempts"] = decoded["Attempts"]
        pub_stats["Received"] = decoded["Received"]
        pub_stats["Missed"] = decoded["Missed"]
        tests["stats"] = True

def on_msg_debug(decoded):
    global debugval
    debugval = decoded["value"]

def on_msg_error(decoded):
    global errors
    errors.append(decoded["Type"])

def on_message(client, data, msg):
    global pub_results, tests, reset, started 
    rcvtime = round(time.time() - starttime,2)
    #print("Message Received @", rcvtime)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
        tests["json"] = True
        if(topic == "/team20/stats"):
            on_msg_stats(decoded)
        elif(topic == "/team20/debug"):
            on_msg_debug(decoded)
        elif(topic == "/team20/errors"):
            on_msg_error(decoded)
        if reset == True:
            reset = False
            started = {item: False for item in topics.keys()}
        if started[topic] == False:
            started[topic] = True
        else:
            if(decoded["ID"] != ID[topic] + 1):
                pub_results[topic]["Failures"] += (decoded["ID"] - (ID[topic]+1))
        ID[topic] = decoded["ID"]
        pub_results[topic]["Successes"] += 1
        pub_results[topic]["Time"] = rcvtime 
        #print("Success:", topic.split("/")[-1], "@", round(time.time() - starttime,2))
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_component():
    global ID
    print("Running test: Component @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    data["value"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_config():
    global ID, tests
    print("Running test: Config @",round(time.time() - starttime,2))
    testval = 5
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    data["value"] = testval
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1)
    if debugval == testval:
        tests["config"] = True

def test_badPayload():
    global tests, ID
    print("Running test: Bad Payload @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {"ID": ID[topic],
            "NULL": 0}
    package = json.dumps(data)
    misses = pub_stats["Missed"]
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    if((pub_stats["Missed"] == (misses + 1)) and errors[-1] == badpayloadval):
        tests["badPayload"] = True

def test_repeatID():
    global ID, tests
    print("Running test: Repeat ID @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    data["value"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data["value"] = 2
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1)
    if(errors[-1] == repeatIDval and debugval == 1):
        tests["repeatID"] = True

def test_skipID():
    global tests, ID
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    misses = pub_stats["Missed"]
    skipval = 2
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    data["value"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += skipval
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    data["value"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    if((pub_stats["Missed"] == (misses + skipval)) and errors[-1] == skipIDval):
        tests["skipID"] = True

def test_noID():
    global ID, tests
    print("Running test: no ID @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {}
    package = json.dumps(data)
    misses = pub_stats["Missed"]
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    if((pub_stats["Missed"] == (misses + 1)) and errors[-1] == noIDval):
        tests["noID"] = True
        

def test_overflowBuf():
    global tests, ID
    print("Running test: overflowBuf @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    data["ID"] = ID[topic]
    for i in range(100):
        data[str(i)] = i
    package = json.dumps(data)
    test_size(package)
    misses = pub_stats["Missed"]
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    if((pub_stats["Missed"] == (misses + 1)) and errors[-1] == overflowval):
        tests["overflow"] = True

def test_reconnect():
    global tests, reset
    print("Running test: reconnect @",round(time.time() - starttime,2))
    diconnected = False
    timeout = 30
    duration = time.time()
    while(not diconnected):
        rcvtime = round(time.time() - starttime,2)
        if(rcvtime - pub_results["/team20/stats"]["Time"] > 3):
            diconnected = True
            reset = True
            print("reconnect: client disconnected @",round(time.time() - starttime,2))
            break
        if(time.time() - duration > timeout):
            print("reconnect: disconnect timeout @",round(time.time() - starttime,2))
            break
    prevtime = pub_results["/team20/stats"]["Time"]
    duration = time.time()
    while(diconnected):
        if(pub_results["/team20/stats"]["Time"] != prevtime):
            disconnected = False
            tests["reconnect"] = True
            print("reconnect: client reconnected @",round(time.time() - starttime,2))
            break
        if(time.time() - duration > timeout):
            print("reconnect: reconnect timeout @",round(time.time() - starttime,2))
            break

def test_receive():
    global tests, ID
    print("Running test: receive @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(test_keys, 0)
    rcvcount = pub_stats["Received"]
    msgcount = 16
    for i in range(msgcount):
        data["ID"] = ID[topic]
        package = json.dumps(data)
        client.publish(topic,package)
        ID[topic] += 1
        time.sleep(.01)
    print("Sent",msgcount,package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(3)
    if(pub_stats["Received"] == rcvcount + msgcount):
        tests["receive"] = True

def test_dos():
    global tests, ID
    print("Running test: DOS @",round(time.time() - starttime,2))
    disconnected = False
    topic = "/team20/config"
    data = {}
    msgcount = 1000
    for i in range(msgcount):
        data["ID"] = ID[topic]
        data["value"] = i
        package = json.dumps(data)
        client.publish(topic,package)
        ID[topic] += 1
    print("Sent",msgcount,package,"to",topic,"@",round(time.time() - starttime,2))
    timeout = 30
    duration = time.time()
    while(not disconnected):
        rcvtime = round(time.time() - starttime,2)
        if(rcvtime - pub_results["/team20/stats"]["Time"] > 3):
            disconnected = True
            tests["dos"] = True
            print("DOS: client disconnected @",round(time.time() - starttime,2))
            break
        if(time.time() - duration > timeout):
            print("DOS: disconnect timeout @",round(time.time() - starttime,2))
            break

def test_send():
    global tests, ID
    print("Running test: send @",round(time.time() - starttime,2))
    dur = 3
    count1 = sum(item["Failures"] for item in pub_results.values())
    time.sleep(dur)
    count2 = sum(item["Failures"] for item in pub_results.values())
    if(count1 == count2):
        tests["send"] = True

def test_time():
    global tests
    print("Running test: time @",round(time.time() - starttime,2))
    count1 = sum(item["Successes"] for item in pub_results.values())
    time.sleep(1.1)
    count2 = sum(item["Successes"] for item in pub_results.values())
    if(count2 - count1 >= 10):
        tests["time"] = True

def test_size(data):
    global tests
    if(len(data.encode("utf-8")) > 256):
        tests["size"] = True

def test_pause():
    global ID, tests
    print("Running test: pause @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {"ID": ID[topic],
            "value": 2}#ROVER_LOADING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(10) #should be paused
    if debugval == 0: #change this to output speed?
        tests["pause"] = True
    data = {"ID": ID[topic],
            "value": 1}#ROVER_MOVING
    package = json.dumps(data)
    client.publish(topic,package)
    ID[topic] += 1
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    #run_tests = ["config", "badPayload", "noID", "repeatID", "skipID", "overflowBuf", "time", "receive", "send", "reconnect", "dos"]
    run_tests = ["pause"]
    for func in run_tests:
        globals()["test_"+func]()
        time.sleep(delay)    
    print("Completed Tests @",round(time.time() - starttime,2))
    print(tests)
    
def display_data():
    print("Thread started: display_data")
    topic = "/team20/display"
    data = {"Stats": 0,
            "Results": 0}   
    while(True):
        if not connected:
            time.sleep(1)
        else:
            data["Stats"] = pub_stats
            data["Results"] = pub_results
            package = json.dumps(data)
            client.publish(topic,package)
            time.sleep(10)
    
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
    threading.Thread(target=display_data).start()
    
    
