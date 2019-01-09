#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Samsung.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRSamsungAc ac(kIrLed);  // Set the GPIO used for sending messages.
IRsend irsend(kIrLed);
uint16_t ac_off[349] = {640, 17770,  3056, 8898,  522, 472,  550, 1436,  554, 438,  560, 434,  560, 434,  560, 432,  556, 438,  554, 438,  554, 438,  554, 1432,  556, 442,  550, 442,  550, 1436,  550, 1458,  474, 520,  516, 1470,  552, 1436,  556, 1428,  560, 1428,  584, 1402,  560, 438,  556, 438,  554, 438,  556, 438,  556, 438,  554, 438,  554, 442,  552, 464,  528, 464,  470, 524,  542, 450,  546, 446,  552, 442,  554, 438,  556, 438,  556, 434,  558, 434,  560, 434,  560, 434,  556, 438,  556, 442,  552, 442,  550, 444,  550, 442,  550, 448,  546, 464,  528, 464,  474, 518,  546, 446,  550, 442,  554, 438,  554, 438,  560, 434,  560, 434,  554, 1432,  556, 1432,  556, 2934,  3030, 8898,  556, 1436,  550, 442,  550, 468,  526, 464,  530, 464,  474, 520,  546, 446,  550, 442,  556, 438,  554, 1432,  554, 438,  554, 440,  554, 1432,  556, 438,  556, 1432,  554, 1436,  552, 1436,  550, 1458,  554, 1432,  474, 1514,  550, 442,  550, 444,  554, 438,  554, 438,  554, 438,  554, 438,  554, 438,  556, 438,  556, 438,  556, 438,  554, 442,  550, 442,  550, 442,  552, 464,  530, 464,  530, 464,  470, 522,  546, 448,  550, 442,  550, 444,  554, 438,  554, 438,  556, 438,  556, 438,  554, 438,  550, 442,  550, 442,  552, 442,  550, 442,  550, 442,  550, 446,  546, 468,  524, 470,  524, 468,  516, 476,  548, 446,  550, 2912,  3052, 8900,  554, 1434,  554, 438,  554, 438,  556, 438,  556, 438,  556, 438,  554, 438,  554, 442,  550, 442,  550, 1438,  550, 464,  530, 464,  474, 1514,  546, 446,  550, 442,  550, 442,  552, 1432,  554, 1432,  554, 1432,  554, 1432,  556, 442,  550, 1436,  552, 442,  550, 1458,  530, 1458,  520, 478,  546, 448,  546, 446,  550, 1434,  554, 1432,  556, 1432,  556, 438,  530, 464,  528, 470,  524, 468,  520, 474,  520, 1466,  522, 472,  522, 494,  500, 1488,  520, 1468,  550, 444,  550, 442,  526, 468,  524, 1462,  524, 468,  526, 468,  526, 468,  526, 468,  526, 468,  524, 474,  520, 472,  520, 472,  522, 498,  494, 1488,  500, 1488,  526};  // SAMSUNG_AC

void printState() {
  // Display the settings.
  Serial.println("Samsung A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kMitsubishiACStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup() {

  ac.begin();
  irsend.begin();
  Serial.begin(9600);
  delay(200);
  
  ac.on();
  Serial.println("Default state of the remote.");
  Serial.println("Setting desired state for A/C.");
  ac.setFan(kSamsungAcFanAuto);
  ac.setMode(kSamsungAcCool);
  ac.setTemp(25);
  ac.setSwing(false);
  printState();

  //Testing send all above configuration
  ac.send();

}

void loop() {

  delay(10000);
  
  //Testing send raw (off) command
  irsend.sendRaw(ac_off,349,38);

}