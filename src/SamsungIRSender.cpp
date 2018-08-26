#include <SamsungIRSender.h>
#include <irCode.h>
#include <Arduino.h>

IRsend irsend(D8);

void SamsungIRSender::begin()
{
    pinMode(D8, OUTPUT);
    irsend.begin();
}

// void SamsungIRSender :: sendOn(){
//         // irsend.sendRaw(ac_on, 116, 36);  // Send a raw data capture at 38kHz.
//         irsend.sendRaw(header,sizeof(header)/sizeof(uint16_t),36);
//         irsend.sendRaw(cool_25_fanLow_notSwing,sizeof(cool_25_fanLow_notSwing)/sizeof(uint16_t),36);
//     }

void SamsungIRSender ::sendOff()
{
    irsend.sendRaw(ac_off, 232, 36); // Send a raw data capture at 38kHz.
}

void SamsungIRSender ::setCoolMode(unsigned int temp, unsigned int fanSpeed = 2, bool isSwing = true, bool isFirstOn = false)
{
    if (temp < 23 && temp > 28)
        return;
    // sendHeaderCode(isFirstOn);

    irsend.sendRaw(header, sizeof(header) / sizeof(uint16_t), 36);
    if (isFirstOn)
    {
        irsend.sendRaw(on, sizeof(on) / sizeof(uint16_t), 36);
    }

    switch (temp)
    {
    case 23:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_23_auto_false, sizeof(cool_23_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_23_auto_true, sizeof(cool_23_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_23_1_false, sizeof(cool_23_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_23_1_true, sizeof(cool_23_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_23_2_false, sizeof(cool_23_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_23_2_true, sizeof(cool_23_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_23_3_false, sizeof(cool_23_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_23_3_true, sizeof(cool_23_3_true) / sizeof(uint16_t), 36);
        break;
    }
    case 24:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_24_auto_false, sizeof(cool_24_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_24_auto_true, sizeof(cool_24_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_24_1_false, sizeof(cool_24_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_24_1_true, sizeof(cool_24_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_24_2_false, sizeof(cool_24_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_24_2_true, sizeof(cool_24_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_24_3_false, sizeof(cool_24_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_24_3_true, sizeof(cool_24_3_true) / sizeof(uint16_t), 36);
        break;
    }
    case 25:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_25_auto_false, sizeof(cool_25_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_25_auto_true, sizeof(cool_25_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_25_1_false, sizeof(cool_25_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_25_1_true, sizeof(cool_25_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_25_2_false, sizeof(cool_25_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_25_2_true, sizeof(cool_25_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_25_3_false, sizeof(cool_25_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_25_3_true, sizeof(cool_25_3_true) / sizeof(uint16_t), 36);
        break;
    }
    case 26:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_26_auto_false, sizeof(cool_26_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_26_auto_true, sizeof(cool_26_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_26_1_false, sizeof(cool_26_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_26_1_true, sizeof(cool_26_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_26_2_false, sizeof(cool_26_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_26_2_true, sizeof(cool_26_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_26_3_false, sizeof(cool_26_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_26_3_true, sizeof(cool_26_3_true) / sizeof(uint16_t), 36);
        break;
    }
    case 27:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_27_auto_false, sizeof(cool_27_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_27_auto_true, sizeof(cool_27_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_27_1_false, sizeof(cool_27_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_27_1_true, sizeof(cool_27_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_27_2_false, sizeof(cool_27_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_27_2_true, sizeof(cool_27_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_27_3_false, sizeof(cool_27_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_27_3_true, sizeof(cool_27_3_true) / sizeof(uint16_t), 36);
        break;
    }
    case 28:
    {
        if (fanSpeed == 0 && isSwing == false)
            irsend.sendRaw(cool_28_auto_false, sizeof(cool_28_auto_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 0 && isSwing == true)
            irsend.sendRaw(cool_28_auto_true, sizeof(cool_28_auto_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == false)
            irsend.sendRaw(cool_28_1_false, sizeof(cool_28_1_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 1 && isSwing == true)
            irsend.sendRaw(cool_28_1_true, sizeof(cool_28_1_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == false)
            irsend.sendRaw(cool_28_2_false, sizeof(cool_28_2_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 2 && isSwing == true)
            irsend.sendRaw(cool_28_2_true, sizeof(cool_28_2_true) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == false)
            irsend.sendRaw(cool_28_3_false, sizeof(cool_28_3_false) / sizeof(uint16_t), 36);
        if (fanSpeed == 3 && isSwing == true)
            irsend.sendRaw(cool_28_3_true, sizeof(cool_28_3_true) / sizeof(uint16_t), 36);
        break;
    }
    }
}

// void sendHeaderCode(bool isFirstOn)
// {
//     irsend.sendRaw(header, sizeof(header) / sizeof(uint16_t), 36);
//     if (isFirstOn)
//     {
//         irsend.sendRaw(on, sizeof(on) / sizeof(uint16_t), 36);
//     }
// }

void setFanMode()
{
    //Send fan mode
    int three = 1 + 2;
}
