#include "SPI.h"
#include "MFRC522.h"
#include "NewPing.h"

#define RST_PIN 9 // RES pin
#define SS_PIN  10 // SDA (SS) pin

byte readCard[4];
String cardID = "16C2764C"; // замените на ID своей метки
String tagID = "";

unsigned long time1, timest, timep, timesp, timeb, timee;
byte start, back, pause;

long duration, cm;

MFRC522 mfrc522(SS_PIN, RST_PIN); // создание объекта mfrc522
NewPing sonar(3, 2, 31);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {
 while (getID()) {
  delay(50);
  unsigned int distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println("см");
    if (tagID == cardID && start == 0 && pause == 0) {
      Serial.println("Access Granted!");
      if (back == 0 && 31 - distance >= 3 && distance <= 29){
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        back = 1;
      }
      delay((30 - distance)*1000/3);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      time1=millis();
    }
      if(millis()-time1 <= 500){ // устраняем дребезг кнопки, блокировка нажатия 200 мс
    pause = 1;
    delay(500);
    pause = 0;
  }
    if (back == 1 && start == 0 && 31 - distance >= 3 && distance <= 29){
        delay(6000);
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
        back = 0;
        delay((30 - distance)*1000/3);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
    }
    else if (tagID != cardID) {
      Serial.println("Access Denied!");
    }
    Serial.print("ID: ");
    Serial.println(tagID);
    delay(2000);
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
