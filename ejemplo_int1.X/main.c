/*
 * Arquitectura de Computadoras
 * Facultad de Ingeniería
 * UNPSJB - Trelew
 *
 * Ejemplo de interrupciones utilizando Change Notification
 */
#include "xc.h"

void __attribute__((interrupt, auto_psv)) _CNInterrupt (void)
{
  IFS1bits.CNIF = 0;
  if (PORTCbits.RC14)
    PORTDbits.RD12 = 1;
  else
    PORTDbits.RD12 = 0;
}

void
ini (void)
{
  CNEN1bits.CN0IE = 1;
  IEC1bits.CNIE = 1;
  TRISC = 0xFFFF;		// Nos interesa el bit 14
  TRISD = 0xEFFF;		// Nos interesa el bit 12
}

int
main (void)
{
  ini ();
  while (1);
}
