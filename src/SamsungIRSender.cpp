#include <SamsungIRSender.h>
#include <irCode.h>


IRsend irsend(15);

void SamsungIRSender::begin(){
    irsend.begin();
}

void SamsungIRSender :: sendOn(){
        irsend.sendRaw(ac_on, 116, 36);  // Send a raw data capture at 38kHz.
    }

void SamsungIRSender :: sendOff(){
        irsend.sendRaw(ac_off, 232, 36);  // Send a raw data capture at 38kHz.
}

