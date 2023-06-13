#include "config.h" 
#include "timer.h"
#include "rgb.h"

void main() {
    rgbInit(); // Inicializa o LED RGB
    timerInit(); // Inicializa o Timer

    for (;;) {

        timerDelay(1000); // Delay
        turnOff(BLUE); // Desliga a cor Azul
        turnOn(RED); // Liga a cor Vermelha

        timerDelay(1000); // Delay
        turnOff(RED); // Desliga a cor Azul
        turnOn(GREEN); // Liga a cor Verde

        timerDelay(1000); // Delay
        turnOff(GREEN); // Desliga a cor Verde
        turnOn(BLUE); // Liga a cor Azul
    }
}
