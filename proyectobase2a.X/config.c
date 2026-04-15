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

#define ValPR1 62500 //0xF424

extern int counterINT0;
extern int counterTimer;

/*
 * Rutina de Atención de la interrupción externa INT0
 */
void __attribute__((interrupt, auto_psv)) _INT0Interrupt( void )
{
	/* reset INT0 interrupt flag */
 	IFS0bits.INT0IF = 0;
        counterINT0++;
        
}

/*
 * Rutina de Atención de la interrupción del Timer1
 */
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
	/* reset Timer 1 interrupt flag */
 	IFS0bits.T1IF = 0;

	//T1CONbits.TON = 0; 	//Deshabilito Timer
	counterTimer ++;

}

/*---------------------------------------------------------------------
  Function Name: Init_Timer1
  Description:   Initialize Timer1
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void Init_Timer1( void )
{
	/* ensure Timer 1 is in reset state */
	T1CON = 0;
	T1CONbits.TCKPS = 2; //Prescaler 256

	/* reset Timer 1 interrupt flag */
 	IFS0bits.T1IF = 0;

 	/* set Timer interrupt priority level */
	IPC0bits.T1IP = 5;

	/* enable Timer interrupt */
 	IEC0bits.T1IE = 1;

	/* set Timer period register */
	PR1 = ValPR1;
	T1CONbits.TON = 1; 	//habilito Timer

}

void Init_INT0( void )
{
    /* reset INT0 interrupt flag */
    IFS0bits.INT0IF = 0;

    /* set INT0 interrupt priority level */
    IPC0bits.INT0IP = 5;

    /* enable INT0 interrupt */
    IEC0bits.INT0IE = 1;
}


void config( void )
{
	//Inicialización de PORTS I/O
	/* set LED pins (RA0-RA7) as outputs */
	TRISA = 0xFF00;
	LATA  = 0xFF00;  //Apago todos los LEDs

        /* Inicializar Interrupción Externa INT0 */
        Init_INT0();

	/* Inicializar Timers necesarios */
	Init_Timer1();

}

