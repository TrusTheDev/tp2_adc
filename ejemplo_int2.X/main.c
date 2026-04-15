/*
 * Arquitectura de Computadoras
 * Facultad de Ingeniería
 * UNPSJB - Trelew
 *
 * Ejemplo de interrupciones de un timer.
 * Propósito del ejemplo: Generar una interrupción del timer1 cada 100ms.
 */
#include "xc.h"

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void ) {
    // Este código se ejecuta cada 100 ms.
    IFS0bits.T1IF = 0;
}

void config(void) {
        //Configurar Timer1
        T1CONbits.TON = 0;
        T1CONbits.TCS = 0;
        T1CONbits.TCKPS = 2; // 1:64
        TMR1 = 0;
        PR1 = 62500;
        //Configurar Interrupción.
        IPC0bits.T1IP = 1; // Prioridad 1
        IFS0bits.T1IF = 0;
        IEC0bits.T1IE = 1;
        T1CONbits.TON = 1; // Start!
 }

int main(void) {
    config();
    while(1) {
        Nop();
        Nop();
        Nop();
    }
}
