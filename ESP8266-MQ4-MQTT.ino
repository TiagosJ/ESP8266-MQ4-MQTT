

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "osman";
const char* password = "osman123";
const char* mqtt_server = "192.168.1.44";

int sensorPin=A0;
int gasValue;
int buzzerPin = D3;

WiFiClient espClient;
PubSubClient client(espClient);



void setup_wifi() {

  delay(100);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  
  if ((char)payload[0] == '1') {
    digitalWrite(buzzerPin, LOW);   
    
    
  } else {
    digitalWrite(buzzerPin, HIGH);  
  }

}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
   
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
     
      client.publish("outTopic", "hello world");
     
      client.subscribe("inTopic");
      client.subscribe("espLED");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(500);
    }
  }
}

void setup() {
  pinMode(buzzerPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(sensorPin,INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {

 gasValue=analogRead(sensorPin);
  Serial.println(gasValue);
  
  
    if     ( gasValue > 300 ) {

    digitalWrite(buzzerPin,LOW);
    Serial.println("ortam gazlı");
    delay(50);   
    char value[6];
      utoa(gasValue , value, 10);
    client.publish("espLDR", value);
  } 
  
  else 
  { 
    digitalWrite(buzzerPin,HIGH);
    Serial.println("ortam gazsız");
    delay(50); 
    reconnect();
  client.loop();

  unsigned long now = millis();
     gasValue=analogRead(sensorPin);
     
    
      char value[6];
      utoa(gasValue , value , 10);
    client.publish("espLDR", value);
     }
     delay(300);
   
  }
  
