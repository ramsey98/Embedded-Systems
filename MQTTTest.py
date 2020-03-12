import paho.mqtt.client as mqtt
import time
import json
import threading

topics = {"/team20/rover_stats": ["ID", "Attempts", "Received", "Expected"],
          "/team20/sensors_stats": ["ID", "Attempts", "Received", "Expected"],
          "/team20/arm_stats": ["ID", "Attempts", "Received", "Expected"],
          "/team20/zumo_stats": ["ID", "Attempts", "Received", "Expected"],
          "/team20/zumo_debug": ["ID", "item1", "item2", "item3"],
          }

pub_attempts = {"/team20/rover_stats": 0,
          "/team20/sensors_stats": 0,
          "/team20/arm_stats": 0,
          "/team20/zumo_stats": 0,
          }

pub_received = {"/team20/rover_stats": 0,
          "/team20/sensors_stats": 0,
          "/team20/arm_stats": 0,
          "/team20/zumo_stats": 0,
          }

pub_expected = {"/team20/rover_stats": 0,
          "/team20/sensors_stats": 0,
          "/team20/arm_stats": 0,
          "/team20/zumo_stats": 0,
          }

pub_success = {"/team20/rover_stats": 0,
          "/team20/sensors_stats": 0,
          "/team20/arm_stats": 0,
          "/team20/zumo_stats": 0,
          "/team20/zumo_debug": 0,
          }

pub_time = {"/team20/rover_stats": 0,
          "/team20/sensors_stats": 0,
          "/team20/arm_stats": 0,
          "/team20/zumo_stats": 0,
          "/team20/zumo_debug": 0,
          }

connected = False
starttime = 0
IP = "192.168.2.1"
PORT = 1883

def on_connect(client, data, flag, rc):
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_message(client, data, msg):
    rcvtime = round(time.time() - starttime,2)
    print("Message Received @", recvtime)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
        pub_success[topic] += 1
        pub_attempt[topic] = decoded["Attempts"]
        pub_receive[topic] = decoded["Received"]
        pub_expected[topic] = decoded["Expected"]
        pub_time[topic] = recvtime
        print("Success:", topic.split("/")[-1], "@", round(time.time() - starttime,2))
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_rover():
    print("Running test: Rover @",round(time.time() - starttime,2))
    topic = "/team20/zumo"
    data = dict(topics[topic])
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_sensors():
    print("Running test: Sensors @",round(time.time() - starttime,2))
    topic = "/team20/zumo"
    data = dict(topics[topic])
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_arm():
    print("Running test: Arm @",round(time.time() - starttime,2))
    topic = "/team20/zumo"
    data = dict(topics[topic])
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_zumo():
    print("Running test: Zumo @",round(time.time() - starttime,2))
    topic = "/team20/rover"
    data = dict(topics[topic])
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_badPayload():
    print("Running test: Bad Payload @",round(time.time() - starttime,2))
    topic = "/team20/rover"
    data = {"NULL": 0}
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def test_skipID():
    print("Running test: Skip ID @",round(time.time() - starttime,2))
    topic = "/team20/rover"
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))
    data["ID"] = 3
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def run_tests():
    print("Thread: Test started @",round(time.time() - starttime,2))
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    test_rover()
    time.sleep(1)
    test_sensors()
    time.sleep(1)
    test_arm()
    time.sleep(1)
    test_zumo()
    time.sleep(1)
    test_badPayload()
    time.sleep(1)
    test_skipID()
    print("Completed Tests @",round(time.time() - starttime,2))

def display_data():
    print("Thread: Output started @",round(time.time() - starttime,2))
    while(not connected):
        time.sleep(1)
    while(True):
        print(pub_attempts)
        print(pub_success)
        print(pub_received)
        print(pub_expected)
        print(pub_time)
        time.sleep(10)
    

def main():
    print("Thread: Connection started @",round(time.time() - starttime,2))
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message
    client.connect(IP,PORT,60)
    client.loop_forever()
    
if __name__ == "__main__":
    starttime = time.time()
    threading.Thread(target=main).start()
    threading.Thread(target=run_tests).start()
    threading.Thread(target=display_data).start()
    
