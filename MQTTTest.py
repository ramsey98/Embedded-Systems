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
         "badPayload": False}

topics = {"/team20/stats": ["ID", "Attempts", "Received", "Expected"],
          "/team20/errors": ["ID", "Error", "Value"]}

if test_component == "rover":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
elif test_component == "sensors":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
elif test_component == "arm":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]
elif test_component == "zumo":
    topics["/team20/debug"] = ["ID", "item1", "item2", "item3"]

pub_results = {topic: {"Successes": 0, "Time": 0} for topic in topics.keys()}
pub_stats = {"Attempts": 0,
             "Received": 0,
             "Expected": 0}

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_stats(decoded):
    pub_stats["Attempts"] = decoded["Attempts"]
    pub_stats["Received"] = decoded["Received"]
    pub_stats["Expected"] = decoded["Expected"]

def on_msg_debug(decoded):
    pass

def on_msg_error(decoded):
    if(decoded["Value"] == 0):
        tests[decoded["Error"]] = False
    else:
        tests[decoded["Error"]] = True

def on_message(client, data, msg):
    rcvtime = round(time.time() - starttime,2)
    print("Message Received @", recvtime)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
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
    topic = "/team20/debug"
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
    topic = "/team20/debug"
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
    topic = "/team20/debug"
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
    topic = "/team20/debug"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_badPayload():
    print("Running test: Bad Payload @",round(time.time() - starttime,2))
    topic = "/team20/debug"
    data = {"NULL": 0}
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_repeatID():
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    topic = "/team20/debug"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_skipID():
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    topic = "/team20/debug"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 3
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_overflowBuf():
    print("Running test: overflowBuf @",round(time.time() - starttime,2))
    topic = "/team20/debug"
    data = dict.fromkeys(topics[topic], 0)
    data["ID"] = 0
    for i in range(100):
        data[str(i)] = i
    package = json.dumps(data)
    client.publish(topic,package)

def test_size(data):
    global tests
    if(len(data.encode("utf-8")) > 256):
        tests["size"] = True

def run_tests():
    print("Thread: Test started @",round(time.time() - starttime,2))
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
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
    test_badPayload()
    time.sleep(1)
    test_repeatID()
    time.sleep(1)
    test_skipID()
    time.sleep(1)
    test_overflowBuf()
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
    
    
