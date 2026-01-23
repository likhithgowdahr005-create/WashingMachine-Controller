/* 
 * File:   washing_machine.h
 * Author: rajshekar
 *
 * Created on January 23, 2026, 11:52 AM
 */
#ifndef WASHING_MACHINE_H
#define WASHING_MACHINE_H

// Screen states
#define WM_POWER_SCREEN        0x10
#define WM_PROGRAM_SCREEN     0x20
#define WM_WATER_SCREEN       0x30
#define WM_START_SCREEN       0x40
#define WM_FUNCTION_SCREEN    0x50
#define WM_COMPLETE_SCREEN    0x60

// Global time (used by ISR)
extern unsigned char min;
extern unsigned char sec;

// Global screen flag (used by main.c)
extern unsigned char screen_flag;

// Main task
void washing_machine_task(unsigned char key);

#endif
