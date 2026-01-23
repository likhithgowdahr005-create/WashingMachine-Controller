#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"
#include "washing_machine.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

static void init_config(void)
{
    init_clcd();
    init_matrix_keypad();
    init_timer2();

    // -------- TURN OFF 7-SEG DIGITS ONLY --------
    TRISC = 0x00;     // Digit control lines
    PORTC = 0x00;    // All DISP1?DISP4 OFF

    PEIE = 1;
    GIE = 1;
}



void main(void)
{
    unsigned char key;

    init_config();

    while (1)
    {
        key = read_matrix_keypad(STATE);
        washing_machine_task(key);
    }
}
