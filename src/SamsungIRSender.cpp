#include <SamsungIRSender.h>
#include <irCode.h>
#include <Arduino.h>


IRsend irsend(D8);

void SamsungIRSender::begin(){
    pinMode(D8,OUTPUT);
    irsend.begin();
}

void SamsungIRSender :: sendOn(){
        irsend.sendRaw(ac_on, 116, 36);  // Send a raw data capture at 38kHz.
    }

void SamsungIRSender :: sendOff(){
        irsend.sendRaw(ac_off, 232, 36);  // Send a raw data capture at 38kHz.
}

void SamsungIRSender :: setCoolMode(unsigned int temp,unsigned int fanSpeed = 2, bool isSwing = true){
    if (temp > 23 || temp < 28)
        return;
    // irsend.sendRaw(ac_mode,232,36);
}

void setFanMode(){
    //Send fan mode
    int three = 1+2;
}
