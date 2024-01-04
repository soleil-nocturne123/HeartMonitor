/* SCRIPT TO SETUP THE EFM8 MICROCONTROLLER TARGET */

char _c51_external_startup (void)
{
    /*
        WATCHDOG: Disable (page 374)
    */
	SFRPAGE = 0x00; // Disable all interrupts
	// Disable software watchdog timer with 2-key sequence
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
  
    /*
        SUPPLY MONITOR CONTROL (page 84, 87)
        RESET SOURCE (page 42)
        The supply monitor cannot generate system resets until it is also selected as a reset source in register RSTSRC.
    */
	VDM0CN |= 0x80; // Supply Monitor Enabled
	RSTSRC = 0x02;

    /*
        PREFETCH ENGINE CONTROL (page 89, 97)
            PFE0CN[7:6]: Reserved    PFE0CN[5:4]: Flash Read Timing PFE0CN[3:0]: Reserved
            This field should be programmed to the smallest allowed value, according to the system clock speed. When transitioning to
            a faster clock speed, program FLRT before changing the clock. When changing to a slower clock speed, change the clock
            before changing FLRT.
    */
	#if (SYSCLK == 48000000L)	
		SFRPAGE = 0x10; // Re-enable interrupts
		PFE0CN  = 0x10; // SYSCLK < 50 MHz.
		SFRPAGE = 0x00; // Disable all interrupts
	#elif (SYSCLK == 72000000L)
		SFRPAGE = 0x10; // Re-enable interrupts
		PFE0CN  = 0x20; // SYSCLK < 75 MHz.
		SFRPAGE = 0x00; // Disable all interrupts
	#endif
	
	/*
	    SYSCLK (page 70, 76, 77): Use Internal Oscillator
	        CLKSEL[7]: Clock Divider Ready   CLKSEL[6:4]: Clock Source Divider   CLKSEL[3]: Reserved CLKSEL[2:0]: Clock Source Select
        	This device family has restrictions when switching to clock sources that are greater than 25 MHz. SYSCLK must be running at a frequency
            of 24 MHz or greater before switching the CLKSL field to HFOSC1. When transitioning from slower clock frequencies, firmware
            should make two writes to CLKSEL.
	*/
	#if (SYSCLK == 12250000L) // Select Clock: HFOSC0 (24.5MHz), Divisor: 2
		CLKSEL = 0x10;
		CLKSEL = 0x10;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
	#elif (SYSCLK == 24500000L) // Select Clock: HFOSC0 (24.5MHz), Divisor: 1
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
	#elif (SYSCLK == 48000000L) // Select Clock: HFOSC1 (72MHz), Divisor: 1.5
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
		CLKSEL = 0x07;
		CLKSEL = 0x07;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
	#elif (SYSCLK == 72000000L) // Select Clock: HFOSC1 (72MHz), Divisor: 1
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
		CLKSEL = 0x03;
		CLKSEL = 0x03;
		while ((CLKSEL & 0x80) == 0); // While clock has not propagated through divider yet.
	#else
		#error SYSCLK must be either 12250000L, 24500000L, 48000000L, or 72000000L
	#endif
	
	// UART0
	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00;
	XBR2     = 0x40; // Enable crossbar and weak pull-ups

	#if (((SYSCLK/BAUDRATE)/(2L*12L))>0xFFL)
		#error Timer 0 reload value is incorrect because (SYSCLK/BAUDRATE)/(2L*12L) > 0xFF
	#endif
	// Configure Uart 0
	SCON0 = 0x10;
	CKCON0 |= 0b_0000_0000; // Timer 1 uses the system clock divided by 12.
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready
	
	return 0;
}