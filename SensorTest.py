import paho.mqtt.client as mqtt
import time
import json
import threading
import statistics

connected = False
starttime = time.time()
IP = "192.168.1.40"
PORT = 1883

topics = {"/team20/pixy": ["ID", "Color", "Distance", "Offset"],
          "/team20/ultrasonic": ["ID", "Distance"],
          "/team20/sensorpolls": ["ID", "PixyPolls", "SensorPolls"],
          "/team20/sensorstate": ["ID", "Type", "Position", "Distance", "Offset"],
          "/team20/stats": ["ID", "Attempts", "Received", "Missed"],
          "/team20/errors": ["ID", "Type"],
          "/team20/debug": ["ID", "value"]
        }

tests = {"SensorPolls": False,              # testing Pixy and US sensor poll frequency
         "MQTTSendSensorState": False,      # testing @ 1 second interval 
         "MQTTSendPixyData": False,         # testing @ 1 second interval 
         "MQTTSendUltrasonicData": False,   # testing @ 1 second interval 
         "PixyObjectDetection" : False,
         "PixyZumoDetection" : False,
         "UltrasonicDistance" : False,
         "PixyDistance" : False,
         "SensorState" : False,
         "StatsAndDebug" : False }

ID = {"/team20/config": 0,
      "/team20/pixy" : 0,
      "/team20/ultrasonic" : 0,
      "/team20/sensorpolls" : 0,
      "/team20/sensorstate" : 0,
      "/team20/stats" : 0,
      "/team20/errors" : 0,
      "/team20/debug" : 0 }

pixyJSON = {"ID" :  0,
            "Color" : 0,
            "Distance" : 0,
            "Offset" : 0 }

ultrasonicJSON = {"ID" :  0,
                  "Distance" : 0 }

pollsJSON = {"ID" :  0,
             "PixyPolls" : 0,
             "SensorPolls" : 0 }

sensorStateJSON = {"ID" :  0,
                   "Type" : 0,
                   "Position" : 0,
                   "Distance" : 0,
                   "Offset" : 0 }

recieveTimes = { "/team20/config": starttime,
                 "/team20/pixy" : starttime,
                 "/team20/ultrasonic" : starttime,
                 "/team20/sensorpolls" : starttime,
                 "/team20/sensorstate" : starttime,
                 "/team20/stats" : starttime,
                 "/team20/errors" : starttime,
                 "/team20/debug" : starttime }

def on_connect(client, data, flag, rc):
    global connected
    print("Connected w/ result", str(rc),"@",round(time.time() - starttime,2))
    for topic in topics.keys():
        client.subscribe(topic)
    connected = True

def on_disconnect(client, data, rc):
    print("Disconnected w/ result", str(rc),"@",round(time.time() - starttime,2))

def on_msg_pixy(decoded):
    global pixyJSON
    pixyJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/pixy"] = rcvtime

def on_msg_ultrasonic(decoded):
    global ultrasonicJSON
    ultrasonicJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/ultrasonic"] = rcvtime
    
def on_msg_sensorpolls(decoded):
    global pollsJSON
    pollsJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/sensorpolls"] = rcvtime

def on_msg_sensorstate(decoded):
    global sensorStateJSON
    sensorStateJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/sensorstate"] = rcvtime

def on_message(client, data, msg):
    global pub_results, tests, reset, started 
    rcvtime = round(time.time() - starttime,2)
    topic = msg.topic
    package = msg.payload.decode()
    decoded = json.loads(package)
    if(set(topics[topic]).issubset(set(decoded.keys()))):
        if(topic == "/team20/pixy"):
            on_msg_pixy(decoded)
        elif(topic == "/team20/ultrasonic"):
            on_msg_ultrasonic(decoded)
        elif(topic == "/team20/sensorpolls"):
            on_msg_sensorpolls(decoded)
        elif(topic == "/team20/sensorstate"):
            on_msg_sensorstate(decoded)
        """
        elif(topic == "/team20/stats"):
            #on_msg_stats
        elif(topic == "/team20/debug"):
            #on_msg_debug(decoded)
        if(topic == "/team20/errors"):
            #on_msg_errors(decoded)
        """
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_SensorPolls():
    global tests, pollsJSON
    print("Running test: SensorPolls @",round(time.time() - starttime,2))
    time.sleep(1)
    if pollsJSON["PixyPolls"] == 5 and pollsJSON["SensorPolls"] == 10:
        tests["SensorPolls"] = True

def test_MQTTSendSensorState():
    global tests, recieveTimes
    last = recieveTimes["/team20/sensorstate"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/sensorstate"]

    if last != most_recent and most_recent - last < 1.05:
        tests["MQTTSendSensorState"] = True

def test_MQTTSendPixyData():
    global tests, sensorstateJSON, recieveTimes
    last = recieveTimes["/team20/pixy"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/pixy"]

    if last != most_recent and most_recent - last < 1.05:
        tests["MQTTSendPixyData"] = True

def test_MQTTSendUltrasonicData():
    global tests, recieveTimes
    last = recieveTimes["/team20/ultrasonic"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/ultrasonic"]

    if last != most_recent and most_recent - last < 1.05:
        tests["MQTTSendUltrasonicData"] = True

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    run_tests = ["SensorPolls", "MQTTSendSensorState", "MQTTSendPixyData", "MQTTSendUltrasonicData"]
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