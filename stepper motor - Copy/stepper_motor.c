//***************************************************************************************
// stepper motor driver program
// rev 1
// Patrick Donovan
// p1.0,1,6,7 hooked to transistors
//p1.3 dir switch
// p1.4 speed pot
//***************************************************************************************

#include <msp430.h>
#include <stdbool.h>

volatile unsigned int dir = 0; //o is forward, 1 will be backward
volatile unsigned int A = 0;
volatile unsigned int B = 0;// volatile to stahp optimization



void delay_ms(unsigned int delay)
{
    while (delay--)
    {
        __delay_cycles(1600);
    }
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	DCOCTL = CALDCO_16MHZ; 			// set internal oscillator at 16MHz
	BCSCTL1 = CALBC1_16MHZ; 		// set internal oscillator at 16MHz
	P1DIR |= 0xc3;					// Set P1.0,1,6,7 to output direction
	P1IE |= 0x08;					//enables interrupts on p1.3(der switch)
	P1SEL &= ~0x08;    //  Port 1 P1.3 (push button)
	P1DIR &= ~0x08;    // Port 1 P1.3 (push button) as input, 0 is input
	P1REN |= 0x08;     // Enable Port P1.3 (push button) pull-up resistor
	_BIS_SR(GIE);     //enable global interrupts! nao



	ADC10CTL1 |= CONSEQ1; //continuous sample mode, MUST BE SET FIRST!
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON + MSC; //sample and hold time, adc on, cont. sample
	ADC10AE0 |= 0x04; // select channel A4
	ADC10CTL0 |= ADC10SC + ENC; // start conversions
	__delay_cycles(1000);                   // Wait for ADC Ref to settle
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start



	volatile unsigned int i;


	volatile unsigned int value = 0;





	volatile unsigned int whileDammit = 1;
	while(whileDammit == 1)
	{
		if(ADC10MEM >= 512)
		{
			value = 200;
		}

		else if(ADC10MEM < 512)
		{
			value = 50;
		}
		volatile unsigned int A = 0;
		volatile unsigned int B = 0;// volatile to stahp optimization
		P1OUT = 0x82;
		volatile unsigned int i = 52;
		for(i;i>0;i--)
		{

			delay_ms(1000);
			if((A == 0 && B == 0))
			{
				A = 0;
				B = 1;
			}

			else if((A == 0 && B == 1))
			{
				A = 1;
				B = 1;
			}

			else if(A == 1 && B == 1)
			{
				A = 1;
				B = 0;
			}

			else if(A == 1 && B == 0)
			{
				A = 0;
				B = 0;
			}


			if(A == 0 && B == 0)
				P1OUT = 0x82;
			if(A == 1 && B == 0)
					P1OUT = 0x81;
			if(A == 1 && B == 1)
					P1OUT = 0x41;
			if(A == 0 && B == 1)
					P1OUT = 0x42;

		}
	}
}

#pragma vector=PORT1_VECTOR
	__interrupt void Port_1()
	{
		dir ^= 1;
		P1IFG &= ~0x08;
	}







