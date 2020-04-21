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

statsJSON = {"ID" :  0,
            "Attempts" : 0,
            "Received" : 0,
            "Missed" : 0 }

errorsJSON = {"ID" :  0,
              "Type" : 0 }

debugJSON = {"ID" :  0,
             "value" : 0 }

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

def on_msg_stats(decoded):
    global statsJSON
    statsJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/stats"] = rcvtime

def on_msg_errors(decoded):
    global errorsJSON
    errorsJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/errors"] = rcvtime

def on_msg_debug(decoded):
    global debugJSON
    debugJSON = decoded
    rcvtime = round(time.time() - starttime,2)
    recieveTimes["/team20/debug"] = rcvtime

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
        elif(topic == "/team20/stats"):
            on_msg_stats(decoded)
        elif(topic == "/team20/debug"):
            on_msg_debug(decoded)
        if(topic == "/team20/errors"):
            on_msg_errors(decoded)
    else:
        print("Error:", topic.split("/")[-1], "@", round(time.time() - starttime,2))

def test_SensorPolls():
    global tests, pollsJSON
    print("\tRunning test: SensorPolls @",round(time.time() - starttime,2))
    time.sleep(1)
    if pollsJSON["PixyPolls"] == 5 and pollsJSON["SensorPolls"] == 10:
        tests["SensorPolls"] = True
    print("\tFinished test: SensorPolls @",round(time.time() - starttime,2))


def test_MQTTSendSensorState():
    global tests, recieveTimes
    print("\tRunning test: MQTTSendSensorState @",round(time.time() - starttime,2))
    last = recieveTimes["/team20/sensorstate"]
    time.sleep(1.1)
    most_recent = recieveTimes["/team20/sensorstate"]

    print("\t" + str(most_recent - last))
    if last != most_recent and most_recent - last < 1.1:
        tests["MQTTSendSensorState"] = True
    print("\tFinished test: MQTTSendSensorState @",round(time.time() - starttime,2))

def test_MQTTSendPixyData():
    global tests, sensorstateJSON, recieveTimes
    print("\tRunning test: MQTTSendPixyData @",round(time.time() - starttime,2))
    last = recieveTimes["/team20/pixy"]
    time.sleep(1.1)
    most_recent = recieveTimes["/team20/pixy"]

    print("\t" + str(most_recent - last))
    if last != most_recent and most_recent - last < 1.1:
        tests["MQTTSendPixyData"] = True
    print("\tFinished test: MQTTSendPixyData @",round(time.time() - starttime,2))

def test_MQTTSendUltrasonicData():
    global tests, recieveTimes
    print("\tRunning test: MQTTSendUltrasonicData @",round(time.time() - starttime,2))
    last = recieveTimes["/team20/ultrasonic"]
    time.sleep(1.1)
    most_recent = recieveTimes["/team20/ultrasonic"]

    print("\t" + str(most_recent - last))
    if last != most_recent and most_recent - last < 1.1:
        tests["MQTTSendUltrasonicData"] = True
    print("\tFinished test: MQTTSendUltrasonicData @",round(time.time() - starttime,2))

def test_PixyObjectDetection():
    global tests, pixyJSON
    
    print("\tRunning test: PixyObjectDetection @",round(time.time() - starttime,2))
    # check red object detection
    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    time.sleep(1)
    if pixyJSON["Color"] != 1:
        return
    time.sleep(1)
    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if pixyJSON["Color"] != 1:
        return

    # check green object detection
    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    time.sleep(1)
    if pixyJSON["Color"] != 2:
        return
    time.sleep(1)
    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if pixyJSON["Color"] != 2:
        return

    # check yellow object detection
    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    time.sleep(1)
    if pixyJSON["Color"] != 3:
        return
    time.sleep(1)
    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if pixyJSON["Color"] != 3:
        return

    tests["PixyObjectDetection"] = True
    print("\tFinished test: PixyObjectDetection @",round(time.time() - starttime,2))


def test_PixyZumoDetection():
    global tests, pixyJSON
    
    print("\tRunning test: PixyZumoDetection @",round(time.time() - starttime,2))
    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    time.sleep(1)
    if pixyJSON["Color"] != 4:
        return
    time.sleep(1)
    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if pixyJSON["Color"] != 4:
        return

    tests["PixyZumoDetection"] = True
    print("\tFinished test: PixyZumoDetection @",round(time.time() - starttime,2))

def test_UltrasonicDistance():
    global tests, ultrasonicJSON
    print("\tRunning test: UltrasonicDistance @",round(time.time() - starttime,2))
    input("\tPlace an object at 25cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 25) > (.04*25):
        return
    input("\tPlace an object at 35cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 35) > (.04*35):
        return
    input("\tPlace an object at 45cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 45) > (.04*45):
        return
    input("\tPlace an object at 55cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 55) > (.04*55):
        return
    input("\tPlace an object at 65cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 65) > (.04*65):
        return
    input("\tPlace an object at 75cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 75) > (.04*75):
        return

    tests["UltrasonicDistance"] = True
    print("\tFinished test: UltrasonicDistance @",round(time.time() - starttime,2))

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    run_tests = ["SensorPolls", "MQTTSendSensorState", "MQTTSendPixyData", "MQTTSendUltrasonicData", "PixyObjectDetection", "PixyZumoDetection", "UltrasonicDistance"]
    for func in run_tests:
        input("Press Enter to continue to test: " + func)
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