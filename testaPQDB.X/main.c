
#define USE_LCD

#define USE_RTC

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
#ifdef USE_RTC
#include "ds1307.h"
#endif
#ifdef USE_LCD

#define LcdWriteCharacter(X) lcdChar(X)

void LcdXY(const char x, const char y) {

    switch (y) {
        case 0:
            lcdCommand(0x80 + x);
            break;
        case 1:
            lcdCommand(0xC0 + x);
            break;
        case 2:
            lcdCommand(0x88 + x);
            break;
        case 3:
        case 4:
            lcdCommand(0xCA + x);
            break;
    }
}

#endif

void main() {
    //RTC e LCD

    serialInit(0);

    //LCD
#ifdef USE_LCD
    lcdInit();
#else
    initializeDisplay(0xC0, 0x07, 0x014);
#endif

#ifdef USE_RTC
    //RTC
    dsInit();
#endif

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

#ifdef USE_RTC
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
#endif
    timerReset(1000);
    for (;;) {

        unsigned int i = 0;
        char k = 0;
        unsigned int j = 0;
        unsigned int l = 0;

        //LM35
        i = adcRead(0);
        i = i - 30;

        LcdXY(0, 0);
        LcdWriteCharacter((i / 10) % 10 + 48);
        LcdWriteCharacter((i / 1) % 10 + 48);


        //RTC e LCD
        if (timerEnded()) {
            timerReset(1000);
#ifdef USE_RTC
            i = getHours();
#else
            i = 12;
#endif
            LcdXY(0, 1);
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);

            LcdWriteCharacter(':');
#ifdef USE_RTC
            i = getMinutes();
#else
            i = 30;
#endif
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);

            LcdWriteCharacter(':');

#ifdef USE_RTC
            i = getSeconds();
#else
            i = 45;
#endif
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);

            LcdXY(0, 2);
#ifdef USE_RTC
            i = getDays();
#else
            i = 2;
#endif
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);
            LcdWriteCharacter('/');
#ifdef USE_RTC
            i = getMonths();
#else
            i = 11;
#endif
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);

            LcdWriteCharacter('/');

#ifdef USE_RTC
            i = getYears();
#else
            i = 23;
#endif
            LcdWriteCharacter((i / 10) % 10 + 48);
            LcdWriteCharacter((i / 1) % 10 + 48);

        }

        LcdXY(0, 4);
        kpDebounce(); //Realiza o Debounce do teclado
        for (k = 0; k < 10; k++) { //Varredura
            if (kpRead() & (1 << k)) { //Identifica qual tecla foi pressionada
                LcdWriteCharacter(k + 48);
                break;
            }
        }

        j = adcRead(2); // Leitura do POT
        l = adcRead(1); //Leitura LDR
        ssdDigit(3, (j / 10) % 10); // Dezena POT
        ssdDigit(2, (j / 100) % 10); // Unidade POT
        ssdDigit(1, (l / 10) % 10); // Dezena LDR
        ssdDigit(0, (l / 100) % 10); // Unidade LDR

        for (j = 0; j < 8; j++) { // Delay
            ssdUpdate();
            timerDelay(5);
        }
    }
}