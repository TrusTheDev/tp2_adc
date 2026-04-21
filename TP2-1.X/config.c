/**********************************************************************
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Sebastián Wahler  23/03/2016  Primer release - Interrupciones
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES: 
*
**********************************************************************/
#include "p33FJ256GP710.h"
#include "config.h"

extern char caracteres[];
//El apuntador apunta a la ultima posición del arreglo donde puso un caracter
extern int apuntador;
extern int length;
int aux;
/*
 * Rutina de Atención de la interrupción externa INT0
 */
void __attribute__((interrupt, auto_psv)) _INT1Interrupt( void )
{
	/* reset INT1 interrupt flag */
 	IFS1bits.INT1IF = 0;
    //El caracter que entra por PORTB
    char c = PORTB;
    
    //Verificar si esta en el rango de caracteres posibles.
    if ((c >= '0' && c <= '9') ||
    c == '+' || c == '-' || c == '*' || c == '/'){
        //Verifica donde esta el apuntador, en el caso de estar al final pone el 
        //caracter al inicio o lo pone en la posicion siguiente
        if(apuntador > length){
            apuntador = 0;
        } else {
            apuntador++;
        }
        caracteres[apuntador] = c;
        
    }
    //borrar al final.    
    //counterINT0++;
}

int incrementT1(int timer){
    return timer += 93750;
}
/*
 * Rutina de Atención de la interrupción del Timer1
 */
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
	/* reset Timer 1 interrupt flag*/
 	IFS0bits.T1IF = 0;
    if(aux != PORTB){
        aux = PORTB;
        PR1 = 93750;
    } else if(PR1 != 562500){
        PR1 = incrementT1(PR1);
    }
}

/*---------------------------------------------------------------------
  Function Name: Init_Timer1
  Description:   Initialize Timer1
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void Init_Timer1( void )
{
//Configurar Timer1
T1CONbits.TON = 0;
T1CONbits.TCS = 0;
T1CONbits.TCKPS = 2; // 1:64
TMR1 = 0;
PR1 = 93750; //150 ms 
//Configurar Interrupción.
IPC0bits.T1IP = 1; // Prioridad 1
IFS0bits.T1IF = 0;
IEC0bits.T1IE = 1;
T1CONbits.TON = 1; //start 
}

void Init_INT1( void )
{
    /* reset INT1 interrupt flag */
    IFS1bits.INT1IF = 0;

    /* set INT1 interrupt priority level */
    IPC5bits.INT1IP = 5;

    /* enable INT1 interrupt */
    IEC1bits.INT1IE = 1;
}
void config( void )
{
	TRISB = 0xFFFF; //Todo como entrada
    aux = TRISB;
    /* Inicializar Interrupción Externa INT1 */
    Init_INT1();
	/* Inicializar Timers necesarios */
	Init_Timer1();

}

