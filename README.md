## Project Overview

The Smart Laundry Dryness Helper is an IoT application that helps users monitor laundry drying conditions using sensors and cloud integration.

## Key Features

Tracks humidity trends to estimate drying quality (DHT11 sensor).

Detects rain events to trigger urgent "bring in laundry" alerts (Rain sensor).

Visual feedback via LEDs:
- Green → Good drying conditions
- Yellow → Drying will be slow
- Red → Rain detected (danger)

Stores sensor data on MongoDB in GCP.

Frontend dashboard displays live and historical data in a web interface.

## Frontend (React)

Install Node.js 18+.

Navigate to frontend folder:
```bash
cd laundry-dashboard
npm install
npm start
```

The dashboard runs on http://localhost:3000.

## Step-by-Step Development Process

**Environment Setup** (Python, Node.js, MongoDB, Mosquitto).

**MQTT Broker Configuration**
- Verified service is active (systemctl status mosquitto)
- Enforced authentication (security.conf)

**Backend Logic**
- Subscribes to laundry/sensor/data topic.
- Validates JSON payloads and stores in MongoDB.

**ESP32 Hardware Logic**
- Reads humidity and rain sensor.
- Decides status (Good / Slow / Rain).
- Publishes JSON payload to MQTT.

**Frontend Dashboard**
- Fetches data via REST API from backend.
- Displays live sensor values and LED-inspired status gradient.
