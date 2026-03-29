#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- WIFI ----------
const char* ssid = "HUAWEI-B535-A157";
const char* password = "7G14LT1GQB3";

// ---------- SERVER ----------
String serverUrl = "http://192.168.8.129:5000/price";

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- DATA ----------
float btc = 0;
float eth = 0;
float gold = 0;
float silver = 0;
float gbp = 0;
float eur = 0;

// ---------- SCROLL ----------
int scrollTop = SCREEN_WIDTH;
int scrollBottom = SCREEN_WIDTH;

unsigned long lastFetch = 0;
const unsigned long fetchInterval = 10000;

// ---------- TEXT ----------
String topLine = "";
String bottomLine = "";

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }

  display.setTextColor(WHITE);
  display.setTextSize(2);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.println(WiFi.localIP());
}

// ---------- FETCH ----------
void fetchData() {

  HTTPClient http;
  http.begin(serverUrl);

  int httpCode = http.GET();

  if (httpCode == 200) {

    String payload = http.getString();
    Serial.println(payload);

    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {

      // MATCH NEW SERVER STRUCTURE EXACTLY
      if (doc["crypto"]["bitcoin"]["usd"])
        btc = doc["crypto"]["bitcoin"]["usd"];

      if (doc["crypto"]["ethereum"]["usd"])
        eth = doc["crypto"]["ethereum"]["usd"];

      if (doc["metals"]["gold"])
        gold = doc["metals"]["gold"];

      if (doc["metals"]["silver"])
        silver = doc["metals"]["silver"];

      if (doc["fx"]["GBP"])
        gbp = doc["fx"]["GBP"];

      if (doc["fx"]["EUR"])
        eur = doc["fx"]["EUR"];

      // TOP (yellow zone)
      topLine = "BTC:" + String(btc,0) +
                " ETH:" + String(eth,0) + "   ";

      // BOTTOM (blue zone)
      bottomLine = "Au:" + String(gold,0) +
                   " Ag:" + String(silver,0) +
                   " GBP:" + String(gbp,2) +
                   " EUR:" + String(eur,2) + "   ";
    } else {
      Serial.println("JSON parse error");
    }

  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  }

  http.end();
}

void loop() {

  // periodic fetch
  if (millis() - lastFetch > fetchInterval) {
    lastFetch = millis();
    fetchData();
  }

  display.clearDisplay();

  display.setTextSize(2);

  // width calculation
  int16_t x1, y1;
  uint16_t wTop, hTop;
  uint16_t wBottom, hBottom;

  display.getTextBounds(topLine, 0, 0, &x1, &y1, &wTop, &hTop);
  display.getTextBounds(bottomLine, 0, 0, &x1, &y1, &wBottom, &hBottom);

  // draw top (yellow zone)
  display.setCursor(scrollTop, 0);
  display.print(topLine);

  // draw bottom (blue zone)
  display.setCursor(scrollBottom, 32);
  display.print(bottomLine);

  display.display();

  // scroll
  scrollTop -= 2;
  scrollBottom -= 2;

  // synchronized reset (prevents overlap)
  int maxWidth = max(wTop, wBottom);

  if (scrollTop < -(maxWidth + 20)) {
    scrollTop = SCREEN_WIDTH;
    scrollBottom = SCREEN_WIDTH;
  }

  delay(50);
}