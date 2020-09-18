#include "PWM.h"
/*________________________________________________________________PWM LIBRARY________________________________________________________*/
#include<reg51.h>
// PWM_Pin
sbit PWM_Pin0 = P2^0;    //RIGHT ENGINE
sbit PWM_Pin1 = P2^1;	//LEFT ENGINE
sbit DIR_Pin0 = P2^2;	//RIGHT ENGINE'S SPIN DIRECTION
sbit DIR_Pin1 = P2^3;	//lEFT ENGINE'S SPIN DIRECTION
// Function declarations
void cct_init(void);
void InitTimer0(void);
void InitPWM(void);
void control_Motor(unsigned int Left_value,int directionL,unsigned int Right_value,int directionR);
// Global variables
unsigned char Left_Engine = 0,Right_Engine = 0;	  // It can have a value from 0 (0% duty cycle) to 255 (100% duty cycle)
unsigned int temp = 0,temp1 = 0;    // Used inside Timer0 ISR
// PWM frequency selector
/* PWM_Freq_Num can have values in between 1 to 257	only
 * When PWM_Freq_Num is equal to 1, then it means highest PWM frequency
 * which is approximately 1000000/(1*255) = 3.9kHz
 * When PWM_Freq_Num is equal to 257, then it means lowest PWM frequency
 * which is approximately 1000000/(257*255) = 15Hz
 *
 * So, in general you can calculate PWM frequency by using the formula
 *     PWM Frequency = 1000000/(PWM_Freq_Num*255)
 */
#define PWM_Freq_Num   1	 // Highest possible PWM Frequency


void control_Motor(unsigned int Left_value,int directionL,unsigned int Right_value,int directionR)
{
	Left_Engine = Left_value;
	Right_Engine = Right_value;
	DIR_Pin0 = directionR;
	DIR_Pin1 = directionL;
} 
// Init CCT function
// Timer0 initialize
void InitTimer(void)
{ 
	TMOD &= 0x00;    // Clear 4bit field for timer0
	TMOD |= 0x11;    // Set timer0 in mode 1 = 16bit mode
	TH0 = TL0 = TH1 = TL1 = 0x00;      // First time value
	ET0 = ET1 = 1;         // Enable Timer0 interrupts;
	EA  = 1;         // Global interrupt enable
	TR1 = TR0 = 1;         // Start Timer 0	AND Timer 1
}

// PWM initialize
void InitPWM(void)
{
	Left_Engine = 0;         // Initialize with 0% duty cycle
	Right_Engine = 0;
	InitTimer();    // Initialize timer0 to start generating interrupts
					 // PWM generation code is written inside the Timer0 ISR
}

// Timer0 ISR
void Timer0_ISR (void) interrupt 1   
{
	TR0 = 0;    // Stop Timer 0

	if(PWM_Pin0)	// if PWM_Pin is high
	{
		PWM_Pin0 = 0;
		temp = (255-Right_Engine)*PWM_Freq_Num;
		TH0  = 0xFF - (temp>>8)&0xFF;
		TL0  = 0xFF - temp&0xFF;	
	}
	else	     // if PWM_Pin is low
	{
		PWM_Pin0 = 1;
		temp = Right_Engine*PWM_Freq_Num;
		TH0  = 0xFF - (temp>>8)&0xFF;
		TL0  = 0xFF - temp&0xFF;
	}

	TF0 = 0;     // Clear the interrupt flag
	TR0 = 1;     // Start Timer 0
}
void Timer1_ISR (void) interrupt 3   
{
	TR1 = 0;    // Stop Timer 0

	if(PWM_Pin1)	// if PWM_Pin is high
	{
		PWM_Pin1 = 0;
		temp1 = (255-Left_Engine)*PWM_Freq_Num;
		TH1  = 0xFF - (temp1>>8)&0xFF;
		TL1  = 0xFF - temp1&0xFF;	
	}
	else	     // if PWM_Pin is low
	{
		PWM_Pin1 = 1;
		temp1 = Left_Engine*PWM_Freq_Num;
		TH1  = 0xFF - (temp1>>8)&0xFF;
		TL1  = 0xFF - temp1&0xFF;
	}
	TF1 = 0;     // Clear the interrupt flag
	TR1 = 1;     // Start Timer 0
}