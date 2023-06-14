
#define USE_LCD

#include "config.h" 
#include "adc.h"
#include "keypad.h"
#include "io.h"
#ifdef USE_LCD
#include "lcd.h"
#else
#include "nokia5110.h"
#endif
#include "console.h"
#include "pwm.h"
#include "rgb.h"
#include "serial.h"
#include "so.h"
#include "ssd.h"
#include "timer.h"
#include "i2c.h"
#include "ds1307.h"

#ifdef USE_LCD

#define LcdWriteCharacter(X) lcdChar(X)

void LcdXY(const char x, const char y) {

    switch (y) {
        case 0:
            lcdCommand(0x80 + x / 6);
            break;
        case 1:
            lcdCommand(0xC0 + x / 6);
            break;
        case 2:
            lcdCommand(0x88 + x / 6);
            break;
        case 3:
        case 4:
            lcdCommand(0xCA + x / 6);
            break;
    }
}

#endif

void main() {
    unsigned int timecont = 0;
    unsigned char rtc_slot = 0;
    //RTC e LCD

    serialInit(0);

    //LCD
#ifdef USE_LCD
    lcdInit();
#else
    initializeDisplay(0xC0, 0x07, 0x014);
#endif


    //RTC
    dsInit();

    //74HC595
    soInit();

    //Teclado
    kpInit();

    //Timer
    timerInit();

    //Conversor AD
    adcInit();

    //Display 7 Seg.
    ssdInit();
    ssdPoint(0, 1);
    ssdPoint(2, 1);

    //RGB
    rgbInit();

    //Buzzer
    pwmInit();

    turnOff(BLUE); // Desliga a cor Azul
    turnOn(RED); // Liga a cor Vermelha
    pwmBuzzer(1000);
    timerDelay(500); // Delay

    turnOff(RED); // Desliga a cor Azul
    turnOn(GREEN); // Liga a cor Verde
    pwmBuzzer(1200);
    timerDelay(500); // Delay

    turnOff(GREEN); // Desliga a cor Verde
    turnOn(BLUE); // Liga a cor Azul
    pwmBuzzer(1500);
    timerDelay(500); // Delay

    pwmBuzzer(0);

#ifdef USE_LCD
    lcdCommand(0x01);
#else
    for (int i = 0; i < 504; i++) LcdWriteData(0x00); // clear LCD
#endif


    dsStartClock();
    /*
    dsWrite(0x57, SEC);
    dsWrite(0x10, MIN);
    dsWrite(0x12, HOUR);
    dsWrite(2, WEEKDAY);
    dsWrite(0x29, DAY);
    dsWrite(5, MONTH);
    dsWrite(0x23, YEAR);
     */

    for (;;) {

        unsigned int i = 0;
        char k = 0;
        unsigned int j = 0;
        unsigned int l = 0;

        //LM35
        i = adcRead(0)*500L / 1023;

        LcdXY(0, 0);
        LcdWriteCharacter((i / 100) % 10 + 48);
        LcdWriteCharacter((i / 10) % 10 + 48);
        LcdWriteCharacter((i / 1) % 10 + 48);
        LcdWriteCharacter('C');

        //RTC e LCD
        if (timecont >= 1000) {
            switch (rtc_slot) {
                case 0:
                    i = getHours();
                    LcdXY(0, 1);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    LcdWriteCharacter(':');
                    break;
                case 1:
                    i = getMinutes();
                    LcdXY(18, 1);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    LcdWriteCharacter(':');
                    break;
                case 2:
                    i = getSeconds();
                    LcdXY(36, 1);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    break;
                case 3:
                    i = getDays();
                    LcdXY(0, 2);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    LcdWriteCharacter('/');
                    break;
                case 4:
                    i = getMonths();
                    LcdXY(18, 2);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    LcdWriteCharacter('/');
                    break;
                case 5:
                    i = getYears();
                    LcdXY(36, 2);
                    LcdWriteCharacter((i / 10) % 10 + 48);
                    LcdWriteCharacter((i / 1) % 10 + 48);
                    break;
                default:
                    timecont -= 1000;
                    rtc_slot = 0xFF;
                    break;
            }
            rtc_slot++;
        }

        LcdXY(0, 4);
        kpDebounce(); //Realiza o Debounce do teclado
        for (k = 0; k < 10; k++) { //Varredura
            if (kpRead() & (1 << k)) { //Identifica qual tecla foi pressionada
                LcdWriteCharacter(k + 48);
                break;
            }
        }

        j = adcRead(2)*50 / 1023; // Leitura do POT
        l = adcRead(1)*50 / 1023; //Leitura LDR

        ssdDigit(3, (j) % 10); // Dezena POT
        ssdDigit(2, (j / 10) % 10); // Unidade POT
        ssdDigit(1, (l) % 10); // Dezena LDR
        ssdDigit(0, (l / 10) % 10); // Unidade LDR

        for (j = 0; j < 8; j++) { // Delay
            ssdUpdate();
            timerDelay(5);
        }
        timecont += 40;
    }
}
