#include "adc.h"
#include "so.h"
#include "ssd.h"
#include "timer.h"

void main() {
    adcInit(); // Inicializa o adc
    soInit(); // Inicializa o 74hc
    ssdInit(); // Inicializa o Display de 7 Segmentos

    unsigned int i;

    for (;;) {
        i = adcRead(0); // Leitura do LM35
        ssdDigit(3, (i / 10) % 10); // Dezena
        ssdDigit(2, (i / 100) % 10); // Unidade
        ssdDigit(1, 0); // 0
        ssdDigit(0, 0); // 0
        for (i = 0; i < 8; i++) { // Delay
            ssdUpdate();
            timerDelay(5);
        }
    }
}
