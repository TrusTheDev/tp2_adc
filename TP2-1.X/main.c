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
 * Objetivo: implemetar rutina de atención cada vez que se recibe un caracter
 * por PORTB de manera circular utilizando un arreglo como buffer que volvera al 
 * inicio una vez que se llegue al final del arreglo.
 * 
 * El ingreso de caracteres se da gracias a un flanco ascendente en la INT1
 * solamente se aceptaran ASCII del '0' al '9' y los simbolos '+' '-' '*' '/' 
 * cualquier otro caracter es descartado.
 * 
 * El consumidor(main) debe leer los caracteres y trasladarlos a una tabla de log
 * de operaciones, en caso de minuscula convertir a mayuscula
 * el programa principal debe ejecutar en paralelo una rutina de parpadeo de un led
 * 
 * Timer
 * El consumidor no es constante, debe configurar un timer para disparar la tarea
 * Estado inicial base: 150us
 * Si no hay datos 150us + 150us hasta un maximo de 900us
 * si hay datos el timer resetea a 150us
 */
char caracteres[5];
int apuntador = 0;
const int length = sizeof(caracteres) / sizeof(caracteres[0]);
int main(void) {
    
    config();
    while(TRUE)
    {
        
    }

}

