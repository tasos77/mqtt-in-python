 
import paho.mqtt.client as paho
import sys
from datetime import datetime


def onMessage(client, userdata,msg):
    if msg.topic == "esp8266/temperature":
        encoding = 'utf-8'
        temp = str(msg.payload.decode(encoding))
        print("Temperature:",float(temp)," ",datetime.now())
    if msg.topic == "esp8266/humidity":
        encoding = 'utf-8'
        humidity = str(msg.payload.decode(encoding))
        print("Humdity:", float(humidity)," ",datetime.now())


client = paho.Client()
client.on_message = onMessage

if client.connect("broker.emqx.io",1883,60) != 0:
    print("Could not connect to MQTT Broker!")
    sys.exit(-1)

client.subscribe("esp8266/humidity")
client.subscribe("esp8266/temperature")
try:
    print("Press CTRL+C to exit...")
    client.loop_forever()
except:
    print("Disconnecting from broker")

client.disconnect()
