#include <p33FJ256GP710.h>
#include "kernel.h"

void init(void)
{

}
//Ojo que el timer esta configurado a partir de ciclos de instruccion., agregar o eliminar
//instrucciones pueden provocar interrumpciones en la rutina de atencion del timer.
//en el timer, provocando bucles infinitos.
void confReloj(void) {
    //Configurar Timer1
    T1CONbits.TON = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS = 0;
    TMR1 = 0;
    PR1 = 100;
    //Configurar Interrupción.
    IPC0bits.T1IP = 1; // Prioridad 1
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1; // Start!
}

//Completo, no tocar
void boot(void)
{
    init();
    confReloj();
    return;
}

int Apuntador = 0;
extern int procesos[];
int *procesoActual;
void planificador(void) {
    procesoActual = WREG15;
    procesos[Apuntador] = procesoActual;
    
    //Reemplazar con operacion ternaria
    if (Apuntador == 2) {
        Apuntador = 0;
    } else {
        Apuntador++;
    }
    WREG15 = procesos[Apuntador];
}

int quantum = 0;
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
    IFS0bits.T1IF = 0;
    quantum++;
    if(quantum == 2){
        quantum = 0;
        planificador();
    }
}
