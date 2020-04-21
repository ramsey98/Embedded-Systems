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

tests = {"MQTTMsgFrequency": False,              # testing @ 1 second interval 
         "UltrasonicDistance" : False,
         "PixyDistance" : False,
         "SensorStateType": False,      
         "SensorStateDistance": False,         
         "SensorStatePosition": False,   
         "SensorStateOffset" : False,
         "SensorStateLighting" : False,
         "SensorStateBackground" : False,
         "SensorStateObstacles" : False }

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

def test_MQTTMsgFrequency():
    global tests, pollsJSON
    success = True
    print("\tRunning test: MQTTMsgFrequency @",round(time.time() - starttime,2))
    time.sleep(1)
    if pollsJSON["PixyPolls"] != 5 or pollsJSON["SensorPolls"] != 10:
        print("\t\tTest Failed")
        success = False

    last = recieveTimes["/team20/sensorstate"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/sensorstate"]
    print("\tTime between sensorstate publish: " + str(most_recent - last))
    if last == most_recent or most_recent - last > 1.1:
        print("\t\tTest Failed")
        success = False
    
    last = recieveTimes["/team20/pixy"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/pixy"]
    print("\tTime between pixy publish: " + str(most_recent - last))
    if last == most_recent or most_recent - last > 1.1:
        print("\t\tTest Failed")
        success = False

    last = recieveTimes["/team20/ultrasonic"]
    time.sleep(1.05)
    most_recent = recieveTimes["/team20/ultrasonic"]
    print("\tTime between ultrasonic publish: " + str(most_recent - last))
    if last == most_recent or most_recent - last > 1.1:
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["MQTTMsgFrequency"] = True 
    print("\tFinished test: MQTTMsgFrequency @",round(time.time() - starttime,2))


def test_UltrasonicDistance():
    global tests, ultrasonicJSON
    success = True
    print("\tRunning test: UltrasonicDistance @",round(time.time() - starttime,2))
    input("\tPlace an object at 25cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an object at 50cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an object at 75cm from the ultrasonic sensor, then press Enter")
    time.sleep(1)
    if abs(ultrasonicJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["UltrasonicDistance"] = True
    print("\tFinished test: UltrasonicDistance @",round(time.time() - starttime,2))

def test_PixyDistance():
    global tests, pixyJSON
    success = True
    print("\tRunning test: PixyDistance @",round(time.time() - starttime,2))
    input("\tPlace an object at 25cm from the Pixy2, then press Enter")
    time.sleep(1)
    if abs(pixyJSON["Distance"] - 25) > (.2*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an object at 50cm from the Pixy2, then press Enter")
    time.sleep(1)
    if abs(pixyJSON["Distance"] - 50) > (.2*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an object at 75cm from the Pixy2, then press Enter")
    time.sleep(1)
    if abs(pixyJSON["Distance"] - 75) > (0.2*75):
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["PixyDistance"] = True
    print("\tFinished test: PixyDistance @",round(time.time() - starttime,2))


def test_SensorStateType():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateType @",round(time.time() - starttime,2))

    input("\tRemove all objects from the playing field, then press Enter")
    if sensorStateJSON["Type"] != 0:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["SensorStateType"] = True
    print("\tFinishing test: SensorStateType @",round(time.time() - starttime,2))

def test_SensorStateDistance():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateDistance @",round(time.time() - starttime,2))

    input("\tRemove all objects from the playing field, then press Enter")
    if sensorStateJSON["Distance"] < 100:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 25cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 50cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle at 75cm from the Pixy2, then press Enter")
    if abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["SensorStateDistance"] = True
    print("\tFinishing test: SensorStateDistance @",round(time.time() - starttime,2))

def test_SensorStatePosition():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStatePosition @",round(time.time() - starttime,2))
    input("\tPlace a red object at 50cm from the Pixy2 to the left, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 0 or sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2 centered, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 1 or sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2 to the right, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 2 or sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False
    
    input("\tPlace a green object at 50cm from the Pixy2 to the left, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 0 or sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2 centered, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 1 or sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2 to the right, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 2 or sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2 to the left, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 0 or sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2 centered, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 1 or sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2 to the right, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 2 or sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2 to the left, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 0 or sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2 centered, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 1 or sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2 to the right, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Position"] != 2 or sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["SensorStatePosition"] = True
    print("\tFinished test: SensorStatePosition @",round(time.time() - starttime,2))

def test_SensorStateOffset():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateOffset @",round(time.time() - starttime,2))
    input("\tPlace a red object at 50cm from the Pixy2 to the left by 8cm, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Offset"] > -6 or sensorStateJSON["Offset"] < -10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2 to the right by 8cm, then press Enter")
    time.sleep(1)
    if sensorStateJSON["Offset"] < 6 or sensorStateJSON["Offset"] > 10:
        print("\t\tTest Failed")
        success = False
    
    input("\tPlace a green object at 50cm from the Pixy2 to the left by 8cm, then press Enter")
    if sensorStateJSON["Offset"] > -6 or sensorStateJSON["Offset"] < -10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2 to the right by 8cm, then press Enter")
    if sensorStateJSON["Offset"] < 6 or sensorStateJSON["Offset"] > 10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2 to the left by 8cm, then press Enter")
    if sensorStateJSON["Offset"] > -6 or sensorStateJSON["Offset"] < -10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2 to the right by 8cm, then press Enter")
    if sensorStateJSON["Offset"] < 6 or sensorStateJSON["Offset"] > 10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2 to the left by 8cm, then press Enter")
    if sensorStateJSON["Offset"] > -6 or sensorStateJSON["Offset"] < -10:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2 to the right by 8cm, then press Enter")
    if sensorStateJSON["Offset"] < 6 or sensorStateJSON["Offset"] > 10:
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["SensorStateOffset"] = True
    print("\tFinished test: SensorStateOffset @",round(time.time() - starttime,2))


def test_SensorStateLighting():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateLighting @",round(time.time() - starttime,2))
    input("\tCycle to next lighting option, then press Enter")
    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False
    
    input("\tCycle to next lighting option, then press Enter")
    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4:
        print("\t\tTest Failed")
        success = False

    input("\tCycle to next lighting option, then press Enter")
    if success == True:
        tests["SensorStateLighting"] = True
    print("\tFinished test: SensorStateLighting @",round(time.time() - starttime,2))
    
def test_SensorStateBackground():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateBackground @",round(time.time() - starttime,2))
    input("\tCycle to next background option, then press Enter")
    input("\tPlace a red object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1 or abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1 or abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a red object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 1 or abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2 or abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2 or abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a green object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 2 or abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3 or abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3 or abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a yellow object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 3 or abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 25cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4 or abs(sensorStateJSON["Distance"] - 25) > (.04*25):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 50cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4 or abs(sensorStateJSON["Distance"] - 50) > (.04*50):
        print("\t\tTest Failed")
        success = False

    input("\tPlace a zumo object at 75cm from the Pixy2, then press Enter")
    if sensorStateJSON["Type"] != 4 or abs(sensorStateJSON["Distance"] - 75) > (.04*75):
        print("\t\tTest Failed")
        success = False
    
    input("\tCycle to next background option, then press Enter")
    if success == True:
        tests["SensorStateBackground"] = True
    print("\tFinished test: SensorStateBackground @",round(time.time() - starttime,2))

def test_SensorStateObstacles():
    global tests, sensorStateJSON
    success = True
    print("\tRunning test: SensorStateObstacles @",round(time.time() - starttime,2))
    input("\tPlace an obstacle1 at 25cm from the Pixy2 with an egg directly behind it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle1 at 50cm from the Pixy2 with an egg in front of it, then press Enter")
    if sensorStateJSON["Type"] > 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle1 at 50cm from the Pixy2 with an egg back to the left of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle1 at 50cm from the Pixy2 with an egg back to the right of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle2 at 25cm from the Pixy2 with an egg directly behind it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle2 at 50cm from the Pixy2 with an egg in front of it, then press Enter")
    if sensorStateJSON["Type"] > 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle2 at 50cm from the Pixy2 with an egg back to the left of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle2 at 50cm from the Pixy2 with an egg back to the right of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle3 at 25cm from the Pixy2 with an egg directly behind it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle3 at 50cm from the Pixy2 with an egg in front of it, then press Enter")
    if sensorStateJSON["Type"] > 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle3 at 50cm from the Pixy2 with an egg back to the left of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle3 at 50cm from the Pixy2 with an egg back to the right of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle4 at 25cm from the Pixy2 with an egg directly behind it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle4 at 50cm from the Pixy2 with an egg in front of it, then press Enter")
    if sensorStateJSON["Type"] > 3:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle4 at 50cm from the Pixy2 with an egg back to the left of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    input("\tPlace an obstacle4 at 50cm from the Pixy2 with an egg back to the right of it, then press Enter")
    if sensorStateJSON["Type"] != 5:
        print("\t\tTest Failed")
        success = False

    if success == True:
        tests["SensorStateObstacles"] = True
    print("\tFinished test: SensorStateObstacles @",round(time.time() - starttime,2))

def run_tests():
    print("Thread started: run_tests")
    delay = 1
    waiting = 0
    while(not connected):
        time.sleep(1)
        print("Waiting for connection:", waiting)
        waiting+=1
    run_tests = ["MQTTMsgFrequency", "UltrasonicDistance", "PixyDistance", "SensorStateType", "SensorStateDistance", "SensorStatePosition", "SensorStateOffset", "SensorStateLighting", "SensorStateBackground", "SensorStateObstacles"]
    # run_tests = ["SensorStateObstacles"]

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