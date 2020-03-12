import paho.mqtt.client as mqtt
import time
import json
import threading

topics = {"/team20/rover": ["ID", "item1", "item2", "item3"],
          "/team20/sensors": ["ID", "item1", "item2", "item3"],
          "/team20/arm": ["ID", "item1", "item2", "item3"],
          "/team20/zumo": ["ID", "item1", "item2", "item3"],
          "/team20/stats": ["ID", "item1", "item2", "item3"],
          } 

connected = False
starttime = 0

def on_connect(client, data, flag, rc):
    print("Connected w/ result", str(rc))
    for item in topics.keys():
        client.subscribe(item)
        client.publish(item,"start")
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc))
    for item in topics.keys():
        client.publish(item,"start")

def on_message(client, data, msg):
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(decoded.keys()) == set(topics[topic])):
        print("Success:", topic.split("/")[-1], "@", round(time.time() - starttime,2))
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_rover():
    print("Running test: Rover")
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
    print("Running test: Sensors")
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
    print("Running test: Arm")
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
    print("Running test: Zumo")
    topic = "/team20/rover"
    data = dict(topics[topic])
    data["ID"] = 0
    data["item1"] = 1
    data["item2"] = 2
    data["item3"] = 3
    package = json.dumps(data)
    client.publish(topic,package)
    print("Sent",package,"to",topic,"@",round(time.time() - starttime,2))

def run_tests():
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    print("Test process started @",round(time.time() - starttime,2))
    test_rover()
    time.sleep(1)
    test_sensors()
    time.sleep(1)
    test_arm()
    time.sleep(1)
    test_zumo()
    print("Completed Tests")

def main():
    print("Connection process started @",round(time.time() - starttime,2))
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message
    client.connect("192.168.2.1",1883,60)
    client.loop_forever()
    
if __name__ == "__main__":
    starttime = time.time()
    threading.Thread(target=main).start()
    threading.Thread(target=run_tests).start()
    
