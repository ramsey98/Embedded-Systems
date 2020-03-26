import paho.mqtt.client as mqtt
import time
import json
import threading

connected = False
starttime = time.time()
IP = "192.168.2.1"
PORT = 1883
test_component = "zumo"

tests = {"size": False,
         "repeatID": False,
         "skipID": False,
         "component": False,
         "badPayload": False,
         "reconnect": False,
         "json": False,
         "stats": False,
         "config": False,
         "overflow": False,
         "time": False}

topics = {"/team20/stats": ["ID", "Attempts", "Received", "Missed"],
          "/team20/errors": ["ID", "Error", "Value"]}

if test_component == "rover":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
    topics["/team20/config"] = ["ID", "item1", "item2", "item3"]
elif test_component == "sensors":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
    topics["/team20/config"] = ["ID", "item1", "item2", "item3"]
elif test_component == "arm":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
    topics["/team20/config"] = ["ID", "item1", "item2", "item3"]
elif test_component == "zumo":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
    topics["/team20/config"] = ["ID", "item1", "item2", "item3"]

pub_results = {topic: {"Successes": 0, "Time": 0} for topic in topics.keys()}
pub_stats = {"Attempts": 0,
             "Received": 0,
             "Missed": 0}

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
    if test_stats(decoded):
        pub_stats["Attempts"] = decoded["Attempts"]
        pub_stats["Received"] = decoded["Received"]
        pub_stats["Missed"] = decoded["Missed"]
        tests["stats"] = True

def on_msg_debug(decoded):
    pass

def on_msg_error(decoded):
    global tests
    if(decoded["Value"] == 0):
        tests[decoded["Error"]] = False
    else:
        tests[decoded["Error"]] = True

def on_message(client, data, msg):
    global pub_results, tests
    rcvtime = round(time.time() - starttime,2)
    print("Message Received @", recvtime)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
        tests["json"] = True
        if(topic == "/team20/stats"):
            on_msg_stats(decoded)
        elif(topic == "team20/debug"):
            on_msg_debug(decoded)
        elif(topic == "team20/errors"):
            on_msg_error(decoded)
        pub_results[topic]["Successes"] += 1
        pub_results[topic]["Time"] = recvtime 
        print("Success:", topic.split("/")[-1], "@", round(time.time() - starttime,2))
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_rover():
    print("Running test: Rover @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_sensors():
    print("Running test: Sensors @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_arm():
    print("Running test: Arm @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_zumo():
    print("Running test: Zumo @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_config():
    print("Running test: Config @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 5
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_badPayload():
    global tests
    print("Running test: Bad Payload @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = {"NULL": 0}
    package = json.dumps(data)
    misses = pub_stats["Missed"]
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    if(pub_stats["Missed"] == (misses + 1)):
        tests["badPayload"] = True

def test_repeatID():
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_skipID():
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 3
    data["item1"] = 1
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_overflowBuf():
    global tests
    print("Running test: overflowBuf @",round(time.time() - starttime,2))
    topic = "/team20/config"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    for i in range(100):
        data[str(i)] = i
    package = json.dumps(data)
    test_size(package)
    misses = pub_stats["Missed"]
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    time.sleep(1.5)
    #check error subscription?
    if(pub_stats["Missed"] == (misses + 1)):
        tests["overflow"] = True

def test_reconnect():
    global tests
    print("Running test: reconnect @",round(time.time() - starttime,2))
    diconnected = False
    duration = time.time()
    while(not diconnected):
        starttime = time.time()
        lasttime = pub_results["/team20/stats"]["Time"]
        if(starttime - lasttime > 3):
            diconnected = True
            print("reconnect: Client Disconnected @",round(time.time() - starttime,2))
        if(time.time() - duration > 15):
            print("reconnect: disconnect timeout @",round(time.time() - starttime,2))
            break
    duration = time.time()
    while(diconnected):
        if(pub_results["/team20/stats"]["Time"] != lasttime):
            disconnected = False
            tests["reconnect"] = True
            print("reconnect: client reconnected @",round(time.time() - starttime,2))
        if(time.time() - duration > 15):
            print("reconnect: reconnect timeout @",round(time.time() - starttime,2))
            break    

def test_size(data):
    global tests
    if(len(data.encode("utf-8")) > 256):
        tests["size"] = True

def test_stats(decoded):
    if(set(topics["/team20/stats"]).issubset(set(decoded.keys()))):
        return True
    else:
        return False

def test_time():
    global tests
    print("Running test: time @",round(time.time() - starttime,2))
    count = pub_results["Success"]
    time.sleep(10)
    if(pub_results["Success"] - count > 10):
        tests["time"] = True

def run_tests():
    print("Thread: Test started @",round(time.time() - starttime,2))
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    test_config()
    time.sleep(1)
    #test_badPayload()
    #time.sleep(1)
    test_repeatID()
    time.sleep(1)
    test_skipID()
    time.sleep(1)
    test_overflowBuf()
    time.sleep(1)
    test_time()
    time.sleep(1)
    test_reconnect()
    if test_component == "rover":
        test_rover()
        time.sleep(1)
    elif test_component == "sensors":
        test_sensors()
        time.sleep(1)
    elif test_component == "arm":
        test_arm()
        time.sleep(1)
    elif test_component == "zumo":
        test_zumo()
        time.sleep(1)
    print("Completed Tests @",round(time.time() - starttime,2))
    print(tests)
    
def display_data():
    print("Thread: Output started @",round(time.time() - starttime,2))
    topic = "/team20/display"
    while(True):
        if not connected:
            time.sleep(1)
        else:
            keys = ["Stats", "Results"]
            data = dict.fromkeys(keys, 0)
            data["Stats"] = pub_stats
            data["Results"] = pub_results
            package = json.dumps(data)
            client.publish(topic,package)
            #print("Stats:", pub_stats)
            #print("Results:", pub_results)
            time.sleep(10)
    
def main():
    global client
    print("Thread: Connection started @",round(time.time() - starttime,2))
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
    
    
