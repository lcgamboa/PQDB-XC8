#include "config.h" 
#include "adc.h"
#include "so.h"
#include "ssd.h"
#include "timer.h"

#include<math.h>

#define GAMMA  0.7
#define RL10  20000.0

void main() {
    double v;
    double Rl;
    unsigned long lux;


    adcInit(); // Inicializa o adc
    soInit(); // Inicializa o 74hc
    ssdInit(); // Inicializa o Display de 7 Segmentos

    unsigned int i;

    for (;;) {
        i = adcRead(1); // Leitura do LDR

        v = (i * 5.0 / 1023.0);
        Rl = ((10000.0 * v) / (3.3 - v));
        lux = (unsigned long) (10.0 * pow(10, log10(RL10 / Rl) / GAMMA));

        if (lux > 1000) {
            lux /= 100;
            ssdPoint(2, 1);
            ssdPoint(0, 0);
        } else {
            ssdPoint(2, 0);
            ssdPoint(0, 1);
        }

        ssdDigit(3, ((lux / 1) % 10));
        ssdDigit(2, ((lux / 10) % 10));
        ssdDigit(1, ((lux / 100) % 10));
        ssdDigit(0, ((lux / 1000) % 10));
        for (i = 0; i < 8; i++) { // Delay
            ssdUpdate();
            timerDelay(5);
        }
    }
}
