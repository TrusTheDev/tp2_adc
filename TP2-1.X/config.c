/**********************************************************************
 * REVISION HISTORY:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author            Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ADDITIONAL NOTES: 
 *
 **********************************************************************/
#include "p33FJ256GP710.h"
#include "config.h"

#define PR1_BASE 750 //150us con prescaler 1:8
#define PR1_MAX 4500 //900us con prescaler 1:8

extern volatile char caracteres[TAM_BUFFER];
extern volatile int indice_escritura;
extern volatile int indice_lectura;
extern volatile int flag_timer;

//Rutina de Atención de la interrupción externa INT1

void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0; //Se limpia el Interrupt Flag para evitar bucle infinito
    char c = PORTB; //Se lee el caracter que entra por PORTB
    //Se verifica que c sea un caracter permitido
    if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '*' || c == '/') {
        caracteres[indice_escritura] = c; //Se guarda en el casillero actual
        indice_escritura++; //Se avanza al próximo casillero

        //Si se llega al tope, vuelve al inicio
        indice_escritura == TAM_BUFFER ? indice_escritura = 0 : 0;
    }
}

//Rutina de Atención de la interrupción del Timer1

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; //Se limpia el Interrupt Flag para evitar bucle infinito
    //Si hay datos
    if (indice_escritura != indice_lectura) {
        flag_timer = 1; //Se avisa que hay datos
        PR1 = PR1_BASE; //Se resetea el PR1 a 150us
    }//Si no hay datos
    else   
    (PR1 < PR1_MAX) ? (PR1 += PR1_BASE) : (PR1 = PR1_MAX);
}

void Init_Timer1(void) {
    //Se configura Timer1
    T1CONbits.TON = 0; //Se asegura que esté apagado para configurarlo
    T1CONbits.TCS = 0; //Reloj interno
    T1CONbits.TCKPS = 1; //Prescaler 1:8
    TMR1 = 0; // Se resetea el cronómetro a 0
    PR1 = PR1_BASE; //Se pone el límite para 150us

    //Se configura la interrupción
    IPC0bits.T1IP = 1; //Se pone prioridad 1 al Timer1
    IFS0bits.T1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
    IEC0bits.T1IE = 1; //Se habilita la interrupción Timer1
    T1CONbits.TON = 1; //Se enciende el Timer1
}

void Init_INT1(void) {
    //Se configura INT1
    IFS1bits.INT1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
    INTCON2bits.INT1EP = 0; //Se configura como flanco ascendente
    IPC5bits.INT1IP = 5; //Se pone prioridad 5 al INT1
    IEC1bits.INT1IE = 1; //Se habilita la interrupción INT1
}

void config(void) {
    AD1PCFGH = 0xFFFF;
    AD1PCFGL = 0xFFFF;
    AD2PCFGL = 0xFFFF;
    //Se configuran los puertos, y se inicializan INT1 y Timer1
    TRISA = 0xFFFE; // Configuración de RA0 como salida para LED
    TRISB = 0xFFFF; //Configuración de PORTB como entrada para recibir datos
    Init_INT1(); //Inicialización de INT1
    Init_Timer1(); //Inicialización de Timer1
}