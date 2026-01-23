#include <xc.h>

extern unsigned char min;
extern unsigned char sec;

void __interrupt() isr(void)
{
    static unsigned int tick = 0;

    if (TMR2IF)
    {
        TMR2IF = 0;   // MUST clear flag

        tick++;

        // With this config, about 1000 ticks ? 1 second
        if (tick >= 1000)
        {
            tick = 0;

            if (sec > 0)
            {
                sec--;
            }
            else if (min > 0)
            {
                min--;
                sec = 59;
            }
        }
    }
}
