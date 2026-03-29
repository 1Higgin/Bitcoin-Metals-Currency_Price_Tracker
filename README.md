ESP32 Live Market Tracker (Crypto + Metals + FX)
Overview

This project builds a real-time market data display system using:

A Python Flask server that aggregates live data
An ESP32 microcontroller
An OLED display (SSD1306)
A scrolling ticker-style UI

The ESP32 fetches data from the Python API and renders it on the OLED in a live updating ticker format.
System Architecture:- [ External APIs ]
        ↓
[ Python Flask Server ]
        ↓  (JSON API: /price)
[ ESP32 Device ]
        ↓
[ OLED Display ]

Components:-  1. Python Server

The server:

Runs a Flask API endpoint (/price)
Continuously fetches:
Cryptocurrency prices (Bitcoin, Ethereum)
Precious metals (Gold, Silver)
FX rates (GBP, EUR)
Stores the latest values in memory
Serves them as JSON to the ESP32
Key Responsibilities:
Data aggregation
Background update loop
API endpoint exposure
2. Data Sources Used
Crypto:
CoinGecko API
Metals:
Gold API (XAU / XAG endpoints)
FX:
ExchangeRate API
3. ESP32 Firmware

The ESP32:

Connects to WiFi
Calls the Python server endpoint periodically
Parses JSON using ArduinoJson
Displays values on OLED
Implements scrolling ticker UI
📊 Data Format (API Response)

The ESP32 consumes JSON in this structure:

{
  "crypto": {
    "bitcoin": { "usd": 66000 },
    "ethereum": { "usd": 3500 }
  },
  "metals": {
    "gold": 2300,
    "silver": 28
  },
  "fx": {
    "GBP": 0.79,
    "EUR": 0.92
  }
}
🖥️ Display Behavior
OLED split into two visual zones:
Top line: Crypto (BTC, ETH)
Bottom line: Metals + FX
Scrolling ticker effect:
Continuous horizontal movement
Synchronized reset (prevents overlap)
Text size increased for readability
🔧 Key Features Implemented
Real-time data updates (every ~10 seconds)
Background threading in Python server
JSON API for ESP32 consumption
Error handling for API failures
Stable metals data source (replaced unreliable API)
Scrolling ticker UI on OLED
Multi-line formatted output
WiFi-based communication between devices
⚠️ Issues Encountered & Fixes
1. Missing Python Dependencies
Issue: ModuleNotFoundError: flask

Fix: Installed via:

pip install flask requests
2. Network Access Issues
Issue: ESP32 could not access server
Fix:
Used correct local IP (hostname -I)

Ensured server runs on:

host="0.0.0.0"
3. SSL Errors from Metals API

Issue:

TLSV1_UNRECOGNIZED_NAME
Fix:
Replaced unreliable API with stable endpoints
4. Zero Values on Display
Cause:
Incorrect API structure or failing endpoints
Fix:
Aligned ESP32 JSON parsing with server structure
5. OLED Overlapping Text
Cause:
Independent scroll resets
Fix:
Synchronized reset of both scrolling lines
Added width-based boundary control
6. Font Readability
Fix:
Increased OLED text size
Adjusted layout spacing between lines
🚀 How to Run
Python Server
pip install flask requests
python3 server.py

Server runs at:

http://<your-ip>:5000/price
ESP32
Update WiFi credentials:
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";
Update server IP:
String serverUrl = "http://192.168.x.x:5000/price";
Upload sketch to ESP32
🔄 Update Cycle
ESP32 fetches data every ~10 seconds
OLED continuously scrolls ticker
Server refreshes external data in background loop
🧩 Possible Future Improvements
Add price change indicators (↑ ↓)
Smooth continuous scrolling (no reset jump)
Add more assets (stocks, commodities)
Switch to color TFT display
Add graphs/sparklines
Cache + rate-limit API calls
Add web dashboard for monitoring
📌 Summary

This project demonstrates a full pipeline:

External APIs → Python aggregation layer → Local API → Embedded device → Visual display

It combines:

Networking
Data parsing
Embedded systems
Real-time UI rendering

Data Sources Used
Crypto:
CoinGecko API
Metals:
Gold API (XAU / XAG endpoints)
FX:
ExchangeRate API

ESP32 Firmware

The ESP32:

Connects to WiFi
Calls the Python server endpoint periodically
Parses JSON using ArduinoJson
Displays values on OLED
Implements scrolling ticker UI

ESP32 Firmware

The ESP32:

Connects to WiFi
Calls the Python server endpoint periodically
Parses JSON using ArduinoJson
Displays values on OLED
Implements scrolling ticker UI:-  {
  "crypto": {
    "bitcoin": { "usd": 66000 },
    "ethereum": { "usd": 3500 }
  },
  "metals": {
    "gold": 2300,
    "silver": 28
  },
  "fx": {
    "GBP": 0.79,
    "EUR": 0.92
  }
}



{
  "crypto": {
    "bitcoin": { "usd": 66000 },
    "ethereum": { "usd": 3500 }
  },
  "metals": {
    "gold": 2300,
    "silver": 28
  },
  "fx": {
    "GBP": 0.79,
    "EUR": 0.92
  }
}

Key Features Implemented
Real-time data updates (every ~10 seconds)
Background threading in Python server
JSON API for ESP32 consumption
Error handling for API failures
Stable metals data source (replaced unreliable API)
Scrolling ticker UI on OLED
Multi-line formatted output
WiFi-based communication between devices


Issues Encountered & Fixes
1. Missing Python Dependencies
Issue: ModuleNotFoundError: flask

Fix: Installed via:-  pip install flask requests


Network Access Issues
Issue: ESP32 could not access server
Fix:
Used correct local IP (hostname -I)

Ensured server runs on:-Network Access Issues
Issue: ESP32 could not access server
Fix:
Used correct local IP (hostname -I)

Ensured server runs on:host="0.0.0.0"

Update Cycle
ESP32 fetches data every ~10 seconds
OLED continuously scrolls ticker
Server refreshes external data in background loop
🧩 Possible Future Improvements
Add price change indicators (↑ ↓)
Smooth continuous scrolling (no reset jump)
Add more assets (stocks, commodities)
Switch to color TFT display
Add graphs/sparklines
Cache + rate-limit API calls
Add web dashboard for monitoring
📌 Summary

This project demonstrates a full pipeline:

External APIs → Python aggregation layer → Local API → Embedded device → Visual display

It combines:

Networking
Data parsing
Embedded systems
Real-time UI rendering


