#include <p33FJ256GP710.h>
#include "kernel.h"

void init(void)
{

}

void confReloj(void)
{

}

void boot(void)
{
    init();
    confReloj();
    return;
}

void planificador(void)
{

}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
    IFS0bits.T1IF = 0;

}
