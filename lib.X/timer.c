// -----------------------------------------------------------------------
//   Copyright (C) Rodrigo Almeida 2010
// -----------------------------------------------------------------------
//   Arquivo: timer.c
//            Biblioteca de operação do timer1 do PIC18F4520
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
#include "timer.h"
#include "bits.h"
#include "io.h"

char timerEnded(void) {
    return bitTst(INTCON, 2);
}

void timerWait(void) {
    while (!bitTst(INTCON, 2));
}

//tempo em micro segundos

void timerReset(const unsigned int tempo) {
    unsigned int ciclos;

    //para placa com 8MHz 1 us = 2 ciclos
    if (tempo <= 500) {
        //divisor 16
        T0CON = 0b10000011;
        ciclos = (tempo * 1000L) >> 3;
    } else if (tempo <= 1000) {
        //divisor 32
        T0CON = 0b10000100;
        ciclos = (tempo * 1000L) >> 4;
    } else if (tempo <= 2000) {
        //divisor 64
        T0CON = 0b10000101;
        ciclos = (tempo * 1000L) >> 5;
    } else if (tempo <= 4000) {
        //divisor 128
        T0CON = 0b10000110;
        ciclos = (tempo * 1000L) >> 6;
    } else {
        //divisor 256
        T0CON = 0b10000111;
        ciclos = (tempo * 1000L) >> 7;
    }

    //overflow acontece com 2^15-1 = 65535 (max unsigned int)
    ciclos = 65535 - ciclos;


    TMR0H = (ciclos >> 8); //salva a parte alta
    TMR0L = (ciclos & 0x00FF); // salva a parte baixa

    bitClr(INTCON, 2); //limpa a flag de overflow
}

void timerInit(void) {
    T0CON = 0b00001000; //configura timer 0 sem prescaler
    bitSet(T0CON, 7); //liga o timer 0
}

void timerDelay(const unsigned int tempo) {
    timerInit();
    timerReset(tempo);
    timerWait();
}