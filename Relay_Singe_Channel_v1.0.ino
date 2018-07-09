#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>

// Update these with values suitable for your network.


//-------------------------------------------------
// Enter All Configuration Details Here below 

#define MQTT_BROKER     "mqtt://dreamziot.com"
#define MQTT_USERNAME   "user_912d9a0b" // change your device mqtt username
#define MQTT_PASSWORD   "password_0b50e7c7" // change your device mqtt password
#define MQTT_TOPIC      "mqtt_65e804b9_test-relay2" // change your device topic
#define MQTT_CLIENT_ID  "mqtt_65e804b9"   //Enter the deviceID within double quote. 

#define WIFI_SSID       "MTC_TECH"
#define WIFI_PASSWORD   "DzT#789^%"

//--------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

#define RELAY_PIN D7 // Relay GPIO
#define LED_PIN D8 //LED GPIO

void setup() {
  pinMode(RELAY_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

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

String jsonString = String((char *)payload);
 StaticJsonBuffer<200> jsonBuffer;
JsonObject& object = jsonBuffer.parseObject(jsonString);
int status = object["relay"];
Serial.println(status);
 if(status==1){   // Here are are checking, If server is sending 1 , turn on buzzer and LED, You can use anything that you setup on your widget.

 digitalWrite(LED_PIN, 1);
 digitalWrite(RELAY_PIN, 1); 
 }
 else{
 digitalWrite(LED_PIN, 0);
 digitalWrite(RELAY_PIN, 0); 
 }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
