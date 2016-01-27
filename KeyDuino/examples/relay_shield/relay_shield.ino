#include "KeyDuino.h"

//Define relay pins
#define RELAY_1 7
#define RELAY_2 6
#define RELAY_3 5
#define RELAY_4 4

#define CHECK_ID true //Change to "true" if you want to check the good ID. If false, every tag will be accepted.


uint8_t RIGHT_UID[4] = { 0XFF, 0XFF, 0XFF, 0XFF }; //The Mifare Classic UID you want to activate relays with.

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

MifareClassicKeyDuino keyDuino;

void changeRelaysState(int state);
boolean UintArrayCompare(uint8_t a[], uint8_t b[], int array_size);

void setup() {
  Serial.begin(115200);
  keyDuino.begin();

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  while (!Serial)
  Serial.println("/!\\ Warning! /!\\");
  Serial.println("Before going on, please check that all relay switches are OFF on the board, then press a key.");
  Serial.println("Otherwise, you may risk to harm the relay shield ...");
  Serial.println("/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\");

  while (!Serial.available());
  while (Serial.available()) Serial.read();

  changeRelaysState(LOW);
  Serial.println("Waiting for Mifare Classic tag.");
}

void loop() {
  uint8_t success;
  success = keyDuino.readTargetID(uid, &uidLength);

  if (success) {
    if (uidLength == 4) {
      Serial.print("Mifare Classic identified: UID: ");
      keyDuino.PrintHex(uid, uidLength);

      if (!CHECK_ID ||  UintArrayCompare(RIGHT_UID, uid, uidLength)) {
        Serial.println("This is the right tag. Activating relays.");
        changeRelaysState(HIGH);
        delay(2000);
        changeRelaysState(LOW);
        delay(1000);
      } else {
        Serial.println("This is not the right tag.");
        delay(500);
      }
    } else {
      Serial.println("Detected tag is not Mifare Classic.");
      delay(500);
    }
  }
}

void changeRelaysState(int state) {
  digitalWrite(RELAY_1, state);
  digitalWrite(RELAY_2, state);
  digitalWrite(RELAY_3, state);
  digitalWrite(RELAY_4, state);
}

boolean UintArrayCompare(uint8_t a[], uint8_t b[], int array_size)
{
  for (int i = 0; i < array_size; i++)
    if (a[i] != b[i])
      return (false);
  return (true);
}
