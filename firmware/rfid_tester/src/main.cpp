#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RFID_CS   5
#define RFID_RST  2

MFRC522 rfid(RFID_CS, RFID_RST);

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23);  // SCK, MISO, MOSI
  rfid.PCD_Init();
  Serial.println("Scan a tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";  // keep the leading zero
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.print("Tag UID: ");
  Serial.println(uid);

  rfid.PICC_HaltA();
  delay(1000);
}
