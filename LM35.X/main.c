#include "config.h" 
#include "adc.h"
#include "so.h"
#include "ssd.h"
#include "timer.h"

void main() {
    adcInit(); // Inicializa o adc
    soInit(); // Inicializa o 74hc
    ssdInit(); // Inicializa o Display de 7 Segmentos
    ssdPoint(2, 1);

    unsigned int i;

    for (;;) {
        i = adcRead(0)*5000L / 1023; // Leitura do LM35
        ssdDigit(3, ((i / 1) % 10));
        ssdDigit(2, ((i / 10) % 10));
        ssdDigit(1, ((i / 100) % 10));
        ssdDigit(0, ((i / 1000) % 10));
        for (i = 0; i < 8; i++) { // Delay
            ssdUpdate();
            timerDelay(5);
        }
    }
}
