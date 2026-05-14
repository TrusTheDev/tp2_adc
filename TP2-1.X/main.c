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

#define TAM_LOG 100 //Tamaño de la tabla final

volatile char caracteres[TAM_BUFFER]; //Arreglo donde se guardan los caracteres válidos que entran
volatile int indice_escritura = 0; //Índice para saber donde escribir el próximo dato
volatile int indice_lectura = 0; //Índice para saber que dato toca leer
volatile int flag_timer = 0; //"Bandera" para que el main sepa cuando trabajar
char log_operaciones[TAM_LOG]; //Tabla final para guardar los datos válidos finales
int indice_log = 0; //Índice para recorrer la tabla final
unsigned long contador = 0; //Variable para controlar la velocidad del LED

/*
 * Programa Principal
 * 
 * Objetivo: Implementar una rutina de atención para recibir caracteres por PORTB,
 * usando un arreglo como buffer circular (vuelve al inicio al llegar al final).
 * 
 * Productor (Interrupción INT1):
 * El ingreso de caracteres se da por un flanco ascendente en la INT1.
 * Solo se aceptan números (ASCII del '0' al '9') y los símbolos '+' '-' '*' '/'. 
 * Cualquier otro caracter es descartado.
 * 
 * Consumidor (Main):
 * Lee los caracteres del buffer y los traslada a una tabla de Log de Operaciones.
 * Si llega a entrar una letra minúscula, la convierte a mayúscula antes de guardarla.
 * En paralelo a esto, se ejecuta una rutina de parpadeo de un LED.
 * 
 * Temporización Dinámica (Timer1):
 * El consumidor no lee de forma constante, un timer le avisa cuándo trabajar.
 * Estado inicial: 150us.
 * Si no hay datos, la espera aumenta de a 150us hasta llegar a un máximo de 900us.
 * Si hay datos para leer, el timer se resetea a los 150us.
 */

/*
 ### N�meros del 1 al 9
| Car�cter | Decimal | Binario (8 bits) |
| :---: | :---: | :---: |
| 1 | 49 | `00110001` |
| 2 | 50 | `00110010` |
| 3 | 51 | `00110011` |
| 4 | 52 | `00110100` |
| 5 | 53 | `00110101` |
| 6 | 54 | `00110110` |
| 7 | 55 | `00110111` |
| 8 | 56 | `00111000` |
| 9 | 57 | `00111001` |

### Operadores Aritm�ticos
| + | Suma | 43 | `00101011` |
| - | Resta | 45 | `00101101` |
| \ | Multiplicaci�n | 42 | `00101010` |
| / | Divisi�n | 47 | `00101111` |

 */



int main(void) {
    config(); //Se inicializa todo
    while (TRUE) {
        //Si el Timer nos avisa que hay datos nuevos
        if (flag_timer == 1) {
            flag_timer = 0; //Se baja la bandera
            char caracter_valido = caracteres[indice_lectura];
            //Si es una minúscula, se pasa a mayúscula
            if (caracter_valido >= 'a' && caracter_valido <= 'z') {
                caracter_valido = caracter_valido - 32;
            }
            //Se guarda el resultado en la tabla final
            log_operaciones[indice_log] = caracter_valido;
            indice_log++;
            indice_lectura++; //Se avanza al próximo casillero
            //Si los índices llegan al máximo, vuelven a cero (Buffer circular)ç//
            indice_log == TAM_LOG ? indice_log = 0 : 0;
            indice_lectura == TAM_BUFFER ? indice_lectura = 0 : 0;
        }
        //Rutina de parpadeo del LED (en paralelo)
        //Si se cumplen tantos ciclos (100000 por ejemplo)
        if (contador == 100000) {
            contador = 0; //Se reinicia el contador
            //Se invierte el estado del pin
            PORTAbits.RA0 = !PORTAbits.RA0;
        }
        contador++; //El contador siempre suma
    }
}
