#include <p33FJ256GP710.h>
#include "kernel.h"

unsigned int procesos[3]; //Arreglo para guardar por donde va cada proceso
int proceso_actual;
int quantum;
extern void procesoA(void);
extern void procesoB(void);
extern void procesoC(void);

void init(void){
    //Se guardan las direcciones de inicio de cada funcion
    procesos[0] = (int)procesoA;
    procesos[1] = (int)procesoB;
    procesos[2] = (int)procesoC;
    proceso_actual = 0; //Se arranca en el proceso A
    quantum = 0;
}

void confReloj(void){
    //Se configura Timer1
	T1CONbits.TON = 0; //Se asegura que esté apagado para configurarlo
    T1CONbits.TCS = 0; //Reloj interno
	T1CONbits.TCKPS = 0; //Prescaler 1:1
    TMR1 = 0; //Se resetea el cronómetro a 0
    PR1 = 300;
    //Se configura la interrupción
	IPC0bits.T1IP = 1; //Se pone prioridad 1 al Timer1
    IFS0bits.T1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
 	IEC0bits.T1IE = 1; //Se habilita la interrupción Timer1
	T1CONbits.TON = 1; //Se enciende el Timer1
 }
void boot(void)
{
    init();
    confReloj();
    return;
}

unsigned int estadoA[12];
unsigned int estadoB[12];
unsigned int estadoC[12];
void planificador(void){
    static int i;
    unsigned int *puntero = (int)WREG15 - 34;
    switch(proceso_actual){
        case 0:
              for(i = 0; i < 12; i++)
                    estadoA[i] = puntero[i];
             proceso_actual = 1;
            break;
        case 1:
            for(i = 0; i < 12; i++)
                estadoB[i] = puntero[i];
            proceso_actual = 2;
            break;
        case 2:
            for(i = 0; i < 12; i++)
                estadoC[i] = puntero[i];
            proceso_actual = 0;
    }
     *puntero = procesos[proceso_actual];
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void ){
    IFS0bits.T1IF = 0;
    quantum++; //Se suma uno al quantum
    //Cuando pasen 2 quantums, se llama al planificador para cambiar de proceso
    if (quantum == 2){
         quantum = 0;
        planificador();
    }
}