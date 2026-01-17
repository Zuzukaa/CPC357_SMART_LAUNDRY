import pymongo
import paho.mqtt.client as mqtt
import json
from datetime import datetime, timezone

// 1. MongoDB Configuration
// Connects to the local database you just installed
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
db = mongo_client["smart_laundry"]
collection = db["sensor_readings"]

// 2. MQTT Configuration
mqtt_broker_address = "35.208.56.72" 
mqtt_topic = "laundry/sensor/data"

# 3. Callback when connected to Broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to Mosquitto Broker")
        client.subscribe(mqtt_topic)
    else:
        print(f"Connection failed with code {rc}")

# 4. Callback when a message arrives (The Logic)
def on_message(client, userdata, message):
    try:
        # Decode the JSON string from ESP32
        payload = message.payload.decode("utf-8")
        data = json.loads(payload) 
        
        # Add a server-side timestamp for accurate charting
        data["timestamp"] = datetime.now(timezone.utc)
        
        # Ingest into MongoDB
        collection.insert_one(data)
        
        print(f"Data Ingested: Temp: {data.get('temperature')}°C | Status: {data.get('status')}")
    except Exception as e:
        print(f"Error processing message: {e}")

# 5. Initialize Client
client = mqtt.Client()

# 6. SECURITY: ADD AUTHENTICATION
client.username_pw_set("laundry_user", "laundry123")

client.on_connect = on_connect
client.on_message = on_message

print("Starting Smart Laundry Backend...")
client.connect(mqtt_broker_address, 1883, 60)

try:
    client.loop_forever()
except KeyboardInterrupt:
    print("\nStopping script...")
    client.disconnect()