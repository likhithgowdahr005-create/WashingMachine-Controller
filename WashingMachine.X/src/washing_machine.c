#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "washing_machine.h"

// -------- GLOBAL TIME (UPDATED BY ISR) --------
unsigned char min = 0;
unsigned char sec = 0;

// -------- SCREEN STATE --------
unsigned char screen_flag = WM_POWER_SCREEN;

// -------- MENU INDEX --------
unsigned char program_index = 0;
unsigned char water_index   = 0;

// -------- PROGRAM LIST --------
const char *programs[] = {
    "Daily",
    "Heavy",
    "Delicates",
    "Whites",
    "Stain wash",
    "Eco cottons",
    "Woolens",
    "Bed sheets",
    "Rinse+Dry",
    "Dry only",
    "Wash only",
    "Aqua store"
};

// -------- WATER LEVEL LIST --------
const char *water_levels[] = {
    "Auto",
    "Low",
    "Medium",
    "High",
    "Max"
};

// -------- DISPLAY TIME --------
void display_time(void)
{
    clcd_putch((min / 10) + '0', LINE2(8));
    clcd_putch((min % 10) + '0', LINE2(9));
    clcd_putch(':', LINE2(10));
    clcd_putch((sec / 10) + '0', LINE2(11));
    clcd_putch((sec % 10) + '0', LINE2(12));
}

// -------- POWER ON SCREEN --------
void wm_power_on_screen(void)
{
    clear_screen();
    clcd_print("Powering ON", LINE2(3));
    clcd_print("Washing Machine", LINE3(1));
    __delay_ms(1000);
    clear_screen();
}

// -------- SET TIME BASED ON PROGRAM --------
void set_time_for_program(void)
{
    // Set base time depending on selected program

    switch (program_index)
    {
        case 0:  // Daily
            min = 0; sec = 50;
            break;

        case 1:  // Heavy
            min = 1; sec = 20;
            break;

        case 2:  // Delicates
            min = 0; sec = 40;
            break;

        case 3:  // Whites
            min = 1; sec = 0;
            break;

        case 4:  // Stain wash
            min = 1; sec = 30;
            break;

        case 5:  // Eco cottons
            min = 1; sec = 10;
            break;

        case 6:  // Woolens
            min = 0; sec = 45;
            break;

        case 7:  // Bed sheets
            min = 1; sec = 40;
            break;

        case 8:  // Rinse+Dry
            min = 0; sec = 55;
            break;

        case 9:  // Dry only
            min = 0; sec = 35;
            break;

        case 10: // Wash only
            min = 0; sec = 30;
            break;

        case 11: // Aqua store
            min = 0; sec = 25;
            break;

        default:
            min = 0; sec = 50;
            break;
    }
}

// -------- MAIN STATE MACHINE --------
void washing_machine_task(unsigned char key)
{
    switch (screen_flag)
    {
        // -------- 1. POWER ON SCREEN --------
        case WM_POWER_SCREEN:

    clcd_print("Press key 5 to", LINE2(0));
    clcd_print("Power ON Wash", LINE3(0));

    if (key == 5)
    {
        // Always show bars when key 5 is pressed
        unsigned char i;

        clear_screen();

        // Top bar
        for (i = 0; i < 16; i++)
        {
            clcd_putch(0xFF, LINE1(i));
            __delay_ms(30);
        }

        clcd_print("Powering ON", LINE2(3));
        clcd_print("Washing Machine", LINE3(1));

        // Bottom bar
        for (i = 0; i < 16; i++)
        {
            clcd_putch(0xFF, LINE4(i));
            __delay_ms(30);
        }

        __delay_ms(1000);
        clear_screen();

        screen_flag = WM_PROGRAM_SCREEN;
    }
    break;

        // -------- 2. PROGRAM SCREEN --------
        case WM_PROGRAM_SCREEN:

            clcd_print("Washing Programs", LINE1(0));
            clcd_putch('*', LINE2(0));
            clcd_print(programs[program_index], LINE2(1));
            clcd_print("SW4:Scroll", LINE4(0));

            if (key == 4)   // Scroll
            {
                program_index++;
                if (program_index >= 12)
                    program_index = 0;

                clear_screen();
            }
            else if (key == 5)  // Select
            {
                clear_screen();
                screen_flag = WM_WATER_SCREEN;
            }
            break;

        // -------- 3. WATER LEVEL SCREEN --------
        case WM_WATER_SCREEN:

            clcd_print("Water Level:", LINE1(0));
            clcd_putch('*', LINE2(0));
            clcd_print(water_levels[water_index], LINE2(1));
            clcd_print("SW4:Scroll", LINE4(0));

            if (key == 4)
            {
                water_index++;
                if (water_index >= 5)
                    water_index = 0;

                clear_screen();
            }
            else if (key == 5)
            {
                clear_screen();
                screen_flag = WM_START_SCREEN;
            }
            break;

        // -------- 4. START SCREEN --------
        case WM_START_SCREEN:

            clcd_print("Press Switch :", LINE1(0));
            clcd_print("SW5: START", LINE2(0));
            clcd_print("SW6: STOP", LINE3(0));

            if (key == 5)   // START
            {
                clear_screen();

                // Set time based on selected program
                set_time_for_program();

                TMR2ON = 1;     // START TIMER

                screen_flag = WM_FUNCTION_SCREEN;
            }
            else if (key == 6)   // STOP
            {
                clear_screen();
                screen_flag = WM_PROGRAM_SCREEN;
            }
            break;

        // -------- 5. FUNCTION SCREEN --------
        case WM_FUNCTION_SCREEN:

            // Door Open Check (SW1 on RB3)
          // Door Open Check using RB3 (active LOW)
            // Door Open Check (SW1 on RB3, active LOW)
// Door Open Check (SW1 on RB3, active LOW)
static unsigned char door_shown = 0;

if (RB3 == 0)   // Door OPEN
{
    TMR2ON = 0;   // Stop timer

    if (door_shown == 0)
    {
        clear_screen();   // Clear only once when door opens
        clcd_print("DOOR : OPEN", LINE2(2));
        clcd_print("Please close", LINE3(2));
        door_shown = 1;
    }

    return;   // Stay here while door is open
}
else   // Door CLOSED
{
    if (door_shown == 1)
    {
        // Door just closed -> clear once before returning to normal screen
        clear_screen();
        door_shown = 0;
    }
    // Continue to normal function screen display
}



            clcd_print("Function - wash", LINE1(0));
            clcd_print("TIME :", LINE2(0));

            display_time();

            clcd_print("SW5: START", LINE3(0));
            clcd_print("SW6: PAUSE", LINE4(0));

            // Pause
            if (key == 6)
            {
                TMR2ON = 0;
            }
            // Resume
            else if (key == 5)
            {
                TMR2ON = 1;
            }

            // Time over
            if (min == 0 && sec == 0)
            {
                TMR2ON = 0;
                clear_screen();
                screen_flag = WM_COMPLETE_SCREEN;
            }
            break;

        // -------- 6. COMPLETION SCREEN --------
        case WM_COMPLETE_SCREEN:
{
    static unsigned char done_once = 0;

    if (done_once == 0)
    {
        clear_screen();
        clcd_print("Program complete", LINE1(0));
        clcd_print("Remove clothes",   LINE2(0));
        clcd_print("Press 5 for Menu", LINE3(0));
        done_once = 1;
    }

    // When user presses SW5, go back to main page
    if (key == 5)
    {
        done_once = 0;                 // reset for next time
        clear_screen();
        screen_flag = WM_POWER_SCREEN; // back to main page
    }
}
break;
    }
}
