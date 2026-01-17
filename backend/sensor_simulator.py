import paho.mqtt.client as mqtt
import json
import time
import random

# --- CONFIGURATION ---
MQTT_BROKER = "35.208.56.72"
MQTT_PORT = 1883
MQTT_TOPIC = "laundry/sensor/data"
MQTT_USER = "laundry_user"
MQTT_PASS = "laundry123"

def simulate_hardware_logic():
    client = mqtt.Client()
    client.username_pw_set(MQTT_USER, MQTT_PASS)

    try:
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        print("✅ Simulator Connected to GCP. Mimicking ESP32 Hardware Logic...\n")

        while True:
            # --- 1. Simulate Sensor Readings ---
            temperature = round(random.uniform(24.0, 33.0), 1)
            humidity = round(random.uniform(50.0, 95.0), 1)
            is_raining = random.choice([True, False, False, False])  # ~25% chance of rain

            # --- 2. Apply Same Smart Logic as ESP32 ---
            if is_raining:
                status = "Bring in laundry now!"
            elif humidity > 70:
                status = "Drying will be slow"
            else:
                status = "Good drying conditions"

            # --- 3. Create JSON Payload (Matches ESP32 Firmware) ---
            payload = {
                "temperature": temperature,
                "humidity": humidity,
                "rain": is_raining,
                "status": status
            }

            # --- 4. Publish to MQTT ---
            client.publish(MQTT_TOPIC, json.dumps(payload))

            print(f"📤 Outgoing Data -> Temp: {temperature}°C | Humidity: {humidity}% | Rain: {is_raining}")
            print(f"💡 Hardware State -> {status}\n")

            time.sleep(5)

    except Exception as e:
        print(f"❌ Connection Error: {e}")

if __name__ == "__main__":
    simulate_hardware_logic()
