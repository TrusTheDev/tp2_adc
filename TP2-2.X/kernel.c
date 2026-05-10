#include <p33FJ256GP710.h>
#include "kernel.h"

int proceso_actual;
int quantum;
extern void procesoA(void);
extern void procesoB(void);
extern void procesoC(void);

extern unsigned int estadoA[TAM_ESTADO];
extern unsigned int estadoB[TAM_ESTADO];
extern unsigned int estadoC[TAM_ESTADO];

void init(void) {
    //Se guardan las direcciones de inicio de cada funcion
    estadoA[0] = (unsigned int)procesoA;
    estadoB[0] = (unsigned int)procesoB;
    estadoC[0] = (unsigned int)procesoC;
    proceso_actual = 0; //Se arranca en el proceso A
    quantum = 0;
}

void confReloj(void) {
    //Se configura Timer1
    T1CONbits.TON = 0; //Se asegura que esté apagado para configurarlo
    T1CONbits.TCS = 0; //Reloj interno
    T1CONbits.TCKPS = 0; //Prescaler 1:1
    TMR1 = 0; //Se resetea el cronómetro a 0
    PR1 = 10000;
    //Se configura la interrupción
    IPC0bits.T1IP = 1; //Se pone prioridad 1 al Timer1
    IFS0bits.T1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
    IEC0bits.T1IE = 1; //Se habilita la interrupción Timer1
    T1CONbits.TON = 1; //Se enciende el Timer1
}

void boot(void) {
    init();
    confReloj();
    return;
}

void planificador(void) {
    static int i;
    unsigned int *puntero = (unsigned int *)(WREG15 - 34);
    
    //Se guarda el estado del proceso actual
    switch (proceso_actual) {
        case 0:
            for (i = 0; i < TAM_ESTADO; i++)
                estadoA[i] = puntero[i];
            proceso_actual = 1; //Pasamos al B
            break;
        case 1:
            for (i = 0; i < TAM_ESTADO; i++)
                estadoB[i] = puntero[i];
            proceso_actual = 2; //Pasamos al C
            break;
        case 2:
            for (i = 0; i < TAM_ESTADO; i++)
                estadoC[i] = puntero[i];
            proceso_actual = 0; //Pasamos al A
    }
    
    //Restaurar el estado del próximo proceso
    switch (proceso_actual) {
        case 0:
            for (i = 0; i < TAM_ESTADO; i++)
                puntero[i] = estadoA[i];
            break;
        case 1:
            for (i = 0; i < TAM_ESTADO; i++)
                puntero[i] = estadoB[i];
            break;
        case 2:
            for (i = 0; i < TAM_ESTADO; i++)
                puntero[i] = estadoC[i];
    }
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    quantum++; //Se suma uno al quantum
    //Cuando pasen 2 quantums, se llama al planificador para cambiar de proceso
    if (quantum == 2) {
        quantum = 0;
        planificador();
    }
}