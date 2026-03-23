/**********************************************************************
 FileName:        main.c
 Dependencies:    p33FJ256GP710.h
 Processor:       dsPIC33F
 Compiler:        MPLAB® C30 v2.01 or higher

 Ejemplo de funcionamiento de:
 	Timer1
        INT0
 	PORTS

 REVISION HISTORY:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Author            Date      Comments on this revision
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Sebastián Wahler  23/03/16  Proyecto base - Interrupciones
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**********************************************************************/

#include "p33FJ256GP710.h"
#include "config.h"


int counterTimer, counterINT0 = 0;

/*
 * Programa Principal
 */
int main(void) {

    config();

    while(TRUE)
    {
        if (counterTimer > 9 )
        {
            LATA = counterINT0;
            counterTimer = 0;
            counterINT0 = 0;
        }
    }

}

