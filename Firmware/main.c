/*
    HEART MONITOR
    AUTHOR: CHANTEL GUO & PHAN HOAI HUONG NGUYEN (SYLVIA)
    DESCRIPTION:
*/

/* LIBRARIES */
#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>

/* SYSTEM DEFINITION */
#define SYSCLK      72000000L  // SYSCLK frequency in Hz
#define BAUDRATE    115200L  // Baud rate of UART in bps

/* LCD DEFINITION */
#define LCD_RS P2_6
// #define LCD_RW Px_x // Not used in this code.  Connect to GND
#define LCD_E  P2_5
#define LCD_D4 P2_4
#define LCD_D5 P2_3
#define LCD_D6 P2_2
#define LCD_D7 P2_1
#define CHARS_PER_LINE 16

/* SUPPORT SCRIPTS */
#include "setup.c"
#include "TimerCounter.c"
#include "LCD.c"

/* MAIN PROGRAM */
void main (void) 
{
    /* VARIABLE DEFINITION */
    float period;
	float heartRate;
	unsigned char overflow_count;
	char buff[17]; // Line to LCD
	
	/* INITIALIZATION */
	LCD_4BIT(); // Configure the LCD
	TIMER0_Init(); // Timer 0

    /* PuTTY INTERFACE */
	waitms(500); // Give PuTTY a chance to start.
	printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	
	/* LCD WELCOME */
	LCDprint("Heart Monitor:  ", 1, 1);
	
	/* CONTINUOUS HEART MONITOR*/
    while (1)
    {		
		// USING TIMER0: Measure the period at which the signal is one and then zero -- Accounting for overflow
		    // STEP 1: Reset the counter
		TL0=0; 
		TH0=0;
		TF0=0;
		overflow_count=0;
		    // STEP 2: Ensure the cycle is correctly captured
		while(P0_1!=0); // Wait for the signal to be zero
		while(P0_1!=1); // Wait for the signal to be one
		    // STEP 3: Start recording
		TR0=1; // Start the timer
		        // STEP 3.1: First "half" of a cycle (signal is one)
		while(P0_1!=0) // Wait for the signal to be zero
		{
			if(TF0==1) // Did the 16-bit timer overflow?
			{
				TF0=0; // Reset the overflow flag
				overflow_count++;
			}
		}
		        // STEP 3.2: Second "half" of a cycle (signal is zero)
		while(P0_1!=1) // Wait for the signal to be one
		{
			if(TF0==1) // Did the 16-bit timer overflow?
			{
				TF0=0;
				overflow_count++;
			}
		}
		    // STEP 4: Stop recording and get the result
		TR0=0; // Stop the timer
		period=(overflow_count*65536.0+TH0*256.0+TL0)*(12.0/SYSCLK); // ms -- ie. Based on the CKCON0 setup, Timer0 runs on SYSCLK/12 -> 1 Timer0 period has 12/SYSCLK sec (page 316)
		heartRate = 60/period; // beats per minute -- Source on how to calculate heart rate: https://www.picotech.com/library/experiment/calculating-heart-rate
		
		// Send to PuTTY
        printf("%f\n", heartRate);
        
        // LCD Display
		sprintf(buff,"%f bpm", heartRate);
		LCDprint(buff, 2, 1);
    }
}