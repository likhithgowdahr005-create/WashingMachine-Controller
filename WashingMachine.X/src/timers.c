#include <xc.h>
#include "timers.h"

void init_timer2(void)
{
    // Clear timer and flags
    TMR2 = 0;
    TMR2IF = 0;

    // Set period
    PR2 = 250;

    // Enable Timer2 interrupt
    TMR2IE = 1;

    // Timer2 Control:
    // Prescaler = 1:16  (T2CKPS = 11)
    // Postscaler = 1:1  (TOUTPS = 0000)
    // Timer OFF initially (TMR2ON = 0)
    T2CON = 0b00000110;   // 0x06
}
