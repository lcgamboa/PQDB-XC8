#include "ssd.h"
#include "so.h"
#include<xc.h>

void main() {
    soInit();
    ssdInit();
    TRISBbits.TRISB5 = 0;
    int cont = 0, seg = 0, minutos = 0, hora = 0;

    for (;;) { //Loop que toma aproximadamente 1,086ms para ser executado.

        PORTBbits.RB5 ^= 1;
        if (cont >= 921) //1,086*921 = 1000,206 ~ 1000ms = 1s
        {
            seg++;
            cont = 0;
        }
        if (seg >= 60) {
            minutos++;
            seg = 0;
        }
        if (minutos >= 60) {
            hora++;
            minutos = 0;
        }
        if (hora >= 24) {
            hora = 0;
        }

        //Exibe o tempo
        ssdDigit(0, (hora / 10) % 10);
        ssdDigit(1, hora % 10);
        ssdDigit(2, (minutos / 10) % 10);
        ssdDigit(3, minutos % 10);

        ssdPoint(1, seg & 0x01);

        cont++;
        ssdUpdate();
    }
}