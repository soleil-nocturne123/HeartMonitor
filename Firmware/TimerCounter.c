/* SCRIPT WITH SUPPORT FUNCTIONS TO INTERACT WITH TIMERS AND COUNTERS */

/*
    TIMER1: configured in setup.c, used for UART0
*/

/*
    TIMER0: used for measuring heart period (page 316, 331, 332)
*/
void TIMER0_Init(void)
{
	TMOD&=0b_1111_0000; // Set the bits of TMOD to zero: Timer/Counter 0: DISABLED gate control, TIMER mode, Mode 0 -- 13-bit Counter/Timer
	TMOD|=0b_0000_0001; // Set last bit of TMOD to one: Timer/Counter 0: Mode 1 -- 16-bit Counter/Timer
	TR0=0; // Stop Timer/Counter 0
}

/*
    TIMER3: used to delay us -- only use low byte (page 337, 339)
*/
void Timer3us(unsigned char us)
{
	unsigned char i; // usec counter
	
	CKCON0|=0b_0100_0000; // Set bit 6 (T3ML) of CKCON0 (page 327) to one: Timer3 low byte uses SYSCLK
	TMR3RL = (-(SYSCLK)/1000000L); // Reload value: Set Timer3 to overflow in 1us.
	
	TMR3L = TMR3RL;                 // Initialize Timer3 for first overflow
	TMR3CN0 = 0x04;                 // Reset everything, clear overflow flag and enable Timer3
	// Count <us> overflows
	for (i = 0; i < us; i++)
	{
		while (!(TMR3CN0 & 0x80));  // Wait for overflow
		TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	}
	TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
}
void waitms (unsigned int ms)
{
	unsigned int j;
	for(j=ms; j!=0; j--)
	{
		Timer3us(250);
		Timer3us(250);
		Timer3us(250);
		Timer3us(250);
	}
}

