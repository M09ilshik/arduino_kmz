#include "SPI.h"
#include "MFRC522.h"

#define RST_PIN 9 // RES pin
#define SS_PIN  10 // SDA (SS) pin

byte readCard[4];
String cardID = "37CEEA32"; // замените на ID своей метки
String tagID = "";

unsigned long time1, timest, timep, timesp, timeb, timee;
byte start, stop, back, pause;

MFRC522 mfrc522(SS_PIN, RST_PIN); // создание объекта mfrc522

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  while (getID()) {
    if (tagID == cardID && start == 0 && stop == 0 && (back == 0 || back == 1) && pause == 0 ) {
      timest = millis();
      Serial.println("Access Granted!");
      start = 1;
      stop = 0;
      back = 0;
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      time1=millis();
    }
      if(millis()-time1 <= 500){ // устраняем дребезг кнопки, блокировка нажатия 200 мс
    pause = 1;
    delay(500);
    pause = 0;
  } 
    else if (tagID == cardID && pause == 0 && start == 1 && stop == 0 && back == 0) {
      timep = millis();
      Serial.println("Access Granted!");
      start = 0;
      stop = 1;
      back = 0;
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      time1=millis();
    }
      if(millis()-time1 <= 500){ // устраняем дребезг кнопки, блокировка нажатия 200 мс
    pause = 1;
    delay(500);
    pause = 0;
  } 
    else if (tagID == cardID && pause == 0 && start == 0 && stop == 1 && back == 0) {
      timesp = millis();
      Serial.println("Access Granted!");
      start = 0;
      stop = 0;
      back = 1;
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      time1=millis();
    }
      if(millis()-time1 <= 500){ // устраняем дребезг кнопки, блокировка нажатия 200 мс
    pause = 1;
    delay(500);
    pause = 0;} 
    else if (tagID != cardID) {
      Serial.println("Access Denied!");
    }
    Serial.print("ID: ");
    Serial.println(tagID);
    delay(2000);
  }

  timeb = timep - timest;
  timee = millis() - timesp;
  if (timee >= timeb){
        digitalWrite(5, LOW);
  }
}

boolean getID() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";
  
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}