// #include <U8g2lib.h>
// #include <Arduino.h>
// #include "ui.h"

// #ifndef DISPLAY_M
// #define DISPLAY_M

// class OLEDDisplayManager
// {
//   public:
//     const int OLED_ckl = D3;
//     const int OLED_sdin = D2;
//     const int OLED_rst = D1;
//     const int OLED_cs = D0;

//     U8G2_SSD1322_NHD_256X64_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/OLED_ckl, /* data=*/OLED_sdin, /* cs=*/OLED_cs, /* reset=*/OLED_rst); // OLED Display


//     void setTemp(unsigned int temp)
//     {
//         u8g2.firstPage();
//         // if (isOn)
//         // {

//         //     char currentTemp[3];
//         //     String(setTemp).toCharArray(currentTemp, 3);
//         //     do
//         //     {
//         //         u8g2.setFont(u8g2_font_logisoso50_tn);
//         //         u8g2.drawStr(14, 57, currentTemp);
//         //         u8g2.setFont(u8g2_font_helvR24_tf);
//         //         u8g2.drawUTF8(89, 57, "°c");

//         //         u8g2.drawXBMP(200, 37, 58, 27, test_page);
//         //         // u8g2.drawXBMP(0,0, u8g2_logo_97x51_width, u8g2_logo_97x51_height, u8g2_logo_97x51_bits);

//         //         // u8g2.setFont(u8g2_font_ncenB10_tr);
//         //         // u8g2.drawStr(0,12,"On");
//         //     } while (u8g2.nextPage());
//         // }
//         // else
//         // {
//         //     do
//         //     {
//         //         u8g2.setFont(u8g2_font_ncenB10_tr);
//         //         u8g2.drawStr(0, 12, "Off");
//         //     } while (u8g2.nextPage());
//         // }
//     }
// }

// #end