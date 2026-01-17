#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// --- WiFi Credentials ---
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// --- MQTT Configuration ---
const char* mqtt_server = "35.208.56.72";
const int mqtt_port = 1883;
const char* mqtt_topic = "laundry/sensor/data";
const char* mqtt_user = "laundry_user";
const char* mqtt_pass = "laundry123";

// --- Sensor Pins ---
#define DHTPIN 4
#define DHTTYPE DHT11
#define RAIN_PIN 34 

// --- LED Pins ---
#define LED_GREEN 12  // Good Drying
#define LED_YELLOW 14 // Slow Drying
#define LED_RED 27    // Rain Detected (Danger)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// --- Function Prototypes ---
void reconnect();

void setup() {
  Serial.begin(115200);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(RAIN_PIN, INPUT);

  dht.begin();

  // --- Connect to WiFi ---
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // --- Setup MQTT ---
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int rainValue = analogRead(RAIN_PIN);
  bool isRaining = (rainValue < 2000);  // Adjust threshold if needed

  String status;

  // --- SMART LOGIC INTERPLAY ---
  if (isRaining) {
    status = "Bring in laundry now!";
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
  } 
  else if (h > 70) {
    status = "Drying will be slow";
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
  } 
  else {
    status = "Good drying conditions";
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
  }

  // --- CLOUD PAYLOAD ---
  String payload = "{";
  payload += "\"temperature\": " + String(t, 1) + ",";
  payload += "\"humidity\": " + String(h, 1) + ",";
  payload += "\"rain\": " + String(isRaining ? "true" : "false") + ",";
  payload += "\"status\": \"" + status + "\"";
  payload += "}";

  client.publish(mqtt_topic, payload.c_str());
  delay(5000);
}

// --- MQTT Reconnect Function ---
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Laundry", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}
