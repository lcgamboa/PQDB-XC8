#include "console.h"
#include "serial.h"

char string[2] = {'\0', '\0'}; //Conversão de char para string(toda string termina com \0)

void __interrupt() isr() {
    serialReceiveIsr();
}

void main() {
    consoleInit(); //Inicializa o lcd em modo console
    serialInit(1); //Inicializa a serial
    consolePrint("123\n456\n789\n"); //Escreve nas duas primeiras linhas

    for (;;) {
        if (serialAvailable()) {
            string[0] = serialRead(); //Lê o buffer de entrada da serial
            if (string[0] == '<') { //Verifica se o caracter < foi escrito
                consoleMoveLine(-1); //Move a linha em -1
            } else if (string[0] == '>') { //Verifica se o caracter < foi escrito
                consoleMoveLine(+1); //Move a linha em +1
            } else { //Caso ambos caracteres nao forem detectados
                consolePrint(string); //Escreve o que recebeu pela serial
            }
            consoleUpdate(); //Atualiza LCD
        }
    }
}
