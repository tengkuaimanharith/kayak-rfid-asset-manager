#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#include "secrets.h"   // WIFI_SSID, WIFI_PASS, PROJECT_ID, API_KEY (not uploaded to GitHub)

const char* READER_ID = "counter-1";   // must match readerId in app/lib/main.dart

#define RFID_CS   5
#define RFID_RST  2
MFRC522 rfid(RFID_CS, RFID_RST);

String lastTag;
unsigned long lastReadMs = 0;
unsigned long seq = 0;
uint32_t bootId;

String readUid() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

void sendTag(const String& tag) {
  WiFiClientSecure client;
  client.setInsecure();  // OK for learning
  HTTPClient http;
  String url = String("https://firestore.googleapis.com/v1/projects/") + PROJECT_ID +
               "/databases/(default)/documents/readers/" + READER_ID + "?key=" + API_KEY;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  seq++;
  String body = String("{\"fields\":{") +
    "\"tag\":{\"stringValue\":\"" + tag + "\"}," +
    "\"seq\":{\"integerValue\":\"" + String(seq) + "\"}," +
    "\"bootId\":{\"integerValue\":\"" + String(bootId) + "\"}}}";
  int code = http.sendRequest("PATCH", body);
  Serial.printf("Sent %s -> HTTP %d\n", tag.c_str(), code);
  if (code != 200) Serial.println(http.getString());
  http.end();
}

void setup() {
  Serial.begin(115200);
  bootId = esp_random();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWi-Fi connected");
  SPI.begin(18, 19, 23);  // SCK, MISO, MOSI
  rfid.PCD_Init();
  Serial.println("Scan a tag...");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  String tag = readUid();
  if (!(tag == lastTag && millis() - lastReadMs < 3000)) {  // ignore double taps
    sendTag(tag);
    lastTag = tag;
  }
  lastReadMs = millis();
  rfid.PICC_HaltA();
}
