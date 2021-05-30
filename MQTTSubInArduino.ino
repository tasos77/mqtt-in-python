#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);


const char* ssid="yourSSID";          //wifi ssid
const char* password="yourPass";      //wifi password
const char* mqttServer="broker.emqx.io"; 
const int mqttPort = 1883;    
const char* mqttUser = "user";            //if you dont have MQTT Username, no need input
const char* mqttPassword ="user";         //if you dont have MQTT Username, no need input

WiFiClient espClient;
PubSubClient client(mqttServer,mqttPort,espClient);

void setup() {

  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.println("Connecting to WiFi...");
    }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer,mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
      Serial.println("Connecting to MQTT...");

      if(client.connect("ESP8266Client",mqttUser,mqttPassword)){
          Serial.println("connected");
        } else {
            Serial.print("failed with state");
            Serial.print(client.state());
            delay(2000);
          }
    }
    
}

void callback(char* topic,byte* payload,unsigned int length){
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    Serial.print("Message:");
    for (int i=0; i<length; i++){
        Serial.print((char)payload[i]);
      }

    Serial.println();
    Serial.println("-------------------------");
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    //check if returns are valid and print the sensor data
    if(isnan(temperature)||isnan(humidity)){
      Serial.println("Failed to read from DHT");
      }
    else {
       Serial.print("Humidity: ");
       Serial.print(humidity);
       Serial.print(" %\t");
       Serial.print("Temperature: ");
       Serial.print(temperature);
       Serial.print(" *C");
       Serial.println("");
      }
  //MQTT can only transmit strings
    Serial.println("Temperature sent!");
    client.connect("ESP8266Client",mqttUser,mqttPassword);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish("esp8266/temperature", String(temperature).c_str());
    
    Serial.println("Humidity sent!");
    client.connect("ESP8266Client",mqttUser,mqttPassword);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish("esp8266/humidity", String(humidity).c_str());

    client.disconnect();
    delay(1000);
    client.loop();
} 
