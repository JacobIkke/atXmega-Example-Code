/*
 * Xmega_20x_hardware_PWM_signals.c
 *
 * Created: 12-4-2023 14:15:54
 * Author : PeetHobby @PeetGaming
 */ 

#define F_CPU 32000000		// 32MHz clock
#include <avr/io.h>
#include <util/delay.h>

void set_clock_32Mhz_asm(){
	asm("LDI R24,0xD8 \n OUT 0x34,R24 \n");  // Handshake 
	asm("LDI R24,0x02 \n STS 0x0050,R24 \n"); // Enable the internal 32MHz oscillator
	asm("LDS R24,0x0051 \n SBRS R24,1 \n RJMP .-8 \n"); // Wait for 32MHz oscillator to stabilize
	asm("LDI R24,0x01 \n STS 0x0060,R24 \n"); // Enable DFLL defaults to calibrate against internal 32Khz clock
	asm("LDI R25,0xD8 \n OUT 0x34,R25 \n "); // Handshake 
	asm("LDI R24,0x01 \n STS 0x0040,R24 \n"); // Switch to 32MHz clock and enables it
	asm("LDS R24,0x0050 \n ANDI R24,0xFE \n STS 0x0050,R24 \n"); // Disable 2Mhz oscillator
}

void timer_TC0_8PWM_init(void){
	PORTC.DIR = 0xFF;
	PORTC.OUTSET = 0x00;
	//PORTC.PIN0CTRL |= PORT_INVEN_bm;		// invert action on a port, high=low and low=high.

	TCC2.CTRLE = TC2_BYTEM_SPLITMODE_gc; // Enable split mode (TC2) 

	TCC2.CTRLB = 1 << TC2_HCMPDEN_bp    // High Byte CompareD Enable 
			| 1 << TC2_HCMPCEN_bp  // High Byte CompareC Enable
			| 1 << TC2_HCMPBEN_bp  // High Byte CompareB Enable
			| 1 << TC2_HCMPAEN_bp  // High Byte CompareA Enable
			| 1 << TC2_LCMPDEN_bp  // Low Byte Compare D Enable
			| 1 << TC2_LCMPCEN_bp  // Low Byte Compare C Enable
			| 1 << TC2_LCMPBEN_bp  // Low Byte Compare B Enable
			| 1 << TC2_LCMPAEN_bp; // Low Byte Compare A Enable
				
	// Enable or disable waveform output
	TCC2.CTRLC = 1 << TC2_HCMPD_bp // High Byte Compare D Waveform Output
			| 1 << TC2_HCMPC_bp // High Byte Compare C Waveform Output 
			| 1 << TC2_HCMPB_bp // High Byte Compare B Waveform Output
			| 1 << TC2_HCMPA_bp // High Byte Compare A Waveform Output
			| 1 << TC2_LCMPD_bp // Low Byte Compare D Waveform Output
			| 1 << TC2_LCMPC_bp // Low Byte Compare C Waveform Output
			| 1 << TC2_LCMPB_bp // Low Byte Compare B Waveform Output
			| 1 << TC2_LCMPA_bp; // Low Byte Compare A Waveform Output

	TCC2.LCNT = 0;		// Set low CNT, default is 0, bottom update
	TCC2.HCNT = 0;		// Set high CNT, default is 0, bottom update
	TCC2.LPER = 255;	// Set period for low compare registers (0-255)
	TCC2.HPER = 255;	// Set period for HIGH compare registers (0-255)

	TCC2.CTRLA = TC2_CLKSEL_DIV256_gc; // This enables the timer clock and choose divider
}

void timer_TD0_8PWM_init(void){
	PORTD.DIRSET = 0xFF;
	PORTD.OUTSET = 0x00;
	//PORTD.PIN0CTRL |= PORT_INVEN_bm;		// invert action on a port, high=low and low=high.
	
	TCD2.CTRLE = TC2_BYTEM_SPLITMODE_gc; // Timer/Counter split into two 8-bit Counters (TC2) 
	
	TCD2.CTRLB = 1 << TC2_HCMPDEN_bp	// High Byte CompareD 
			| 1 << TC2_HCMPCEN_bp	// High Byte CompareC 
			| 1 << TC2_HCMPBEN_bp	// High Byte CompareB 
			| 1 << TC2_HCMPAEN_bp	// High Byte CompareA 
			| 1 << TC2_LCMPDEN_bp	// Low Byte Compare D 
			| 1 << TC2_LCMPCEN_bp	// Low Byte Compare C 
			| 1 << TC2_LCMPBEN_bp	// Low Byte Compare B 
			| 1 << TC2_LCMPAEN_bp;	// Low Byte Compare A 

	// Enable or disable waveform output
	TCD2.CTRLC = 1 << TC2_HCMPD_bp		// High Byte Compare D Waveform Output 
			| 1 << TC2_HCMPC_bp	// High Byte Compare C Waveform Output 
			| 1 << TC2_HCMPB_bp	// High Byte Compare B Waveform Output 
			| 1 << TC2_HCMPA_bp	// High Byte Compare A Waveform Output 
			| 1 << TC2_LCMPD_bp	// Low Byte Compare D Waveform Output 
			| 1 << TC2_LCMPC_bp	// Low Byte Compare C Waveform Output 
			| 1 << TC2_LCMPB_bp	// Low Byte Compare B Waveform Output  
			| 1 << TC2_LCMPA_bp;	// Low Byte Compare A Waveform Output 


	TCD2.LCNT = 0;		// Set low CNT, default is 0, bottom update
	TCD2.HCNT = 0;		// Set high CNT, default is 0, bottom update
	TCD2.LPER = 255;	// Set period for low compare registers (0-255)
	TCD2.HPER = 255;	// Set period for HIGH compare registers (0-255)

	TCD2.CTRLA = TC2_CLKSEL_DIV256_gc; // This enables the timer clock and choose divider
}

void timer_TCE_4PWM_init(){
	PORTE.DIRSET = 0x0F;
	PORTE.OUTSET = 0x00;

	TCE0.CTRLB = 1 << TC0_CCDEN_bp 
			| 1 << TC0_CCCEN_bp 
			| 1 << TC0_CCBEN_bp 
			| 1 << TC0_CCAEN_bp 
			| TC_WGMODE_SS_gc;  // Single Slope 

	TCE0.CTRLC = 1 << TC0_CMPD_bp   
			| 1 << TC0_CMPC_bp  // Compare Output enable 
			| 1 << TC0_CMPB_bp 
			| 1 << TC0_CMPA_bp;

	TCE0.CNT = 0x0000; 
	TCE0.PER = 0xffff; 
	TCE0.CTRLA = TC_CLKSEL_DIV2_gc; 
}

int main(void){
	
	set_clock_32Mhz_asm();
	timer_TC0_8PWM_init();
	timer_TD0_8PWM_init();
	timer_TCE_4PWM_init();

	uint8_t cnt0 = 0;
	uint8_t cnt_dir = 1;
	uint16_t cnt1;
	
    	while (1){	
		
		//8 x 8-bit PWM signals
		TCC2.LCMPA = cnt0; // PC0 = PWM1
		TCC2.LCMPB = cnt0; // PC1 = PWM2
		TCC2.LCMPC = cnt0; // PC2 = PWM3
		TCC2.LCMPD = cnt0; // PC3 = PWM4

		TCC2.HCMPA = cnt0; // PC4 = PWM5
		TCC2.HCMPB = cnt0; // PC5 = PWM6
		TCC2.HCMPC = cnt0; // PC6 = PWM7
		TCC2.HCMPD = cnt0; // PC7 = PWM8

		//8 x 8-bit PWM signals
		TCD2.LCMPA = cnt0; // PD0 = PWM9
		TCD2.LCMPB = cnt0; // PD1 = PWM10
		TCD2.LCMPC = cnt0; // PD2 = PWM11
		TCD2.LCMPD = cnt0; // PD3 = PWM12	

		TCD2.HCMPA = cnt0; // PD4 = PWM13
		TCD2.HCMPB = cnt0; // PD5 = PWM14
		TCD2.HCMPC = cnt0; // PD6 = PWM15
		TCD2.HCMPD = cnt0; // PD7 = PWM16

		//4 x 16-bit PWM signals
		TCE0.CCA = cnt1; //PE0 = PWM17
		TCE0.CCB = cnt1; //PE1 = PWM18
		TCE0.CCC = cnt1; //PE2 = PWM19
		TCE0.CCD = cnt1; //PE3 = PWM20

		cnt0++;
		cnt1 = 256 * cnt0; 
		_delay_ms(30);

		if (cnt0 >= 192){
			cnt0 = 0;
		}
    	}
}

// All the PWM compare registers 
/*
	TCC2.LCMPA = 127; // PC0 = PWM1
	TCC2.LCMPB = 127; // PC1 = PWM2
	TCC2.LCMPC = 127; // PC2 = PWM3
	TCC2.LCMPD = 127; // PC3 = PWM4
	TCC2.HCMPA = 127; // PC4 = PWM5
	TCC2.HCMPB = 127; // PC5 = PWM6
	TCC2.HCMPC = 127; // PC6 = PWM7
	TCC2.HCMPD = 127; // PC7 = PWM8

	TCD2.LCMPA = 127; // PD0 = PWM9
	TCD2.LCMPB = 127; // PD1 = PWM10
	TCD2.LCMPC = 127; // PD2 = PWM11
	TCD2.LCMPD = 127; // PD3 = PWM12
	TCD2.HCMPA = 127; // PD4 = PWM13
	TCD2.HCMPB = 127; // PD5 = PWM14
	TCD2.HCMPC = 127; // PD6 = PWM15
	TCD2.HCMPD = 127; // PD7 = PWM16
	
	TCE0.CCA = 0x7fff; //PE0 = PWM17
	TCE0.CCB = 0x7fff; //PE1 = PWM18
	TCE0.CCC = 0x7fff; //PE2 = PWM19
	TCE0.CCD = 0x7fff; //PE3 = PWM20
	
	*/
