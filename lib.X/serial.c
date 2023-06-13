// -----------------------------------------------------------------------
//   Copyright (C) Rodrigo Almeida 2010
// -----------------------------------------------------------------------
//   Arquivo: serial.c
//            Biblioteca da porta serial (USART) do PIC18F4520
//   Autor:   Rodrigo Maximiano Antunes de Almeida
//            rodrigomax at unifei.edu.br
//   Licença: GNU GPL 2
// -----------------------------------------------------------------------
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; version 2 of the License.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// -----------------------------------------------------------------------

#include <xc.h>
#include "serial.h"
#include "io.h"
#include "bits.h"

#define BSIZE 32

static char use_int = 0;
static char buffer[BSIZE];
static char bp = 0;

void serialReceiveIsr(void) {
    if (bitTst(PIR1, 5)) {
        buffer[bp] = RCREG; //retorna o valor
        bp++;
        if (bp >= BSIZE) {
            bp = 0;
        }
    }
}

void serialSend(unsigned char c) {
    while (!bitTst(PIR1, 4)); //aguarda o registro ficar disponível
    TXREG = c; //coloca o valor para ser enviado
}

void serialPrint(const char * str) {
    int i = 0;
    while (str[i]) {
        serialSend(str[i++]);
    }
}

void serialPrintln(const char * str) {
    serialPrint(str);
    serialPrint("\r\n");
}

unsigned char serialAvailable(void) {
    if (use_int) {
        return bp;
    }

    return bitTst(PIR1, 5);
}

unsigned char serialRead(void) {

    char resp = 0;

    if (use_int) {
        if (bp) {
            resp = buffer[0];
            bp--;
            for (unsigned char i = 0; i < bp; i++) {
                buffer[i] = buffer[i + 1];
            }
        }
    } else {
        if (bitTst(RCSTA, 1)) //Verifica se há erro de overrun e reseta a serial
        {
            bitClr(RCSTA, 4);
            bitSet(RCSTA, 4);
        }

        if (bitTst(PIR1, 5)) //Verifica se existe algum valor disponivel
        {
            resp = RCREG; //retorna o valor
        }
    }
    return resp; //retorna zero
}

void serialInit(char use_interrupt) {
    use_int = use_interrupt;
    SPBRGH = 0; //configura para 56k
    SPBRG = 34; //configura para 56k
    BAUDCON = 0b00001000; //configura sistema de velocidade da serial
    TXSTA = 0b00101100; //configura a transmissão de dados da serial
    RCSTA = 0b10010000; //configura a recepção de dados da serial

    bitSet(TRISC, 6); //pino de recepção de dados
    bitSet(TRISC, 7); //pino de envio de dados
    if (use_int) {
        bitSet(INTCON, 7);
        bitSet(INTCON, 6);
        bitClr(PIR1, 5);
        bitSet(PIE1, 5);
    }
}

