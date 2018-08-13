#include <SamsungIRSender.h>
#include <irCode.h>


IRsend irsend(15);

SamsungIRSender::SamsungIRSender(){
    irsend.begin();
}

void SamsungIRSender :: sendOn(){
         irsend.sendRaw(on, 67, 36);  // Send a raw data capture at 38kHz.
    }

