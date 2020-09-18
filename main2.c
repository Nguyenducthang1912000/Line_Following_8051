#include <regx52.h>
#include "..\8051 Library\PWM.h"
sbit rstpin = P3^2 ;
void delay_ms (unsigned int t)
{
	unsigned int x,y;
	for(x=0;x<t;x++)
	{
		for(y=0;y<123;y++); 
	}
}
int main(void)
{
   P1=0X1F;
   InitPWM();              // Start PWM
   control_Motor(150,0,150,0);
	while(1)                // Rest is done in Timer0 interrupt
 {  
	 	switch(P1)
		{
		   case 0x00:
			control_Motor(190,1,190,1);
			break;
		   //go straight
		   case 0x04:
		   	control_Motor(100,0,100,0);
			break;
		   //easy turn
		   case 0x08:
		    control_Motor(225,1,80,0);
			break;
		   case 0x02:
		   	control_Motor(80,0,225,1);
			break;
		   case 0x0C:
			control_Motor(220,1,110,0);
			break;
		   case 0x06:
		    control_Motor(110,0,220,1);
			break;
		   //normal right
			case 0x03:
		    control_Motor(110,0,215,1);
			break;
		   //normal left
			case 0x18:
			control_Motor(210,1,110,0);
			break;
		   //super right 
		   case 0x01:
		    control_Motor(120,0,210,1);
			break;
		   //super left
		   case 0x10:
		    control_Motor(210,1,120,0);
			break;
		   case 0x1c:
		    control_Motor(245,1,120,0);
			break;
		   case 0x07:
		    control_Motor(120,0,245,1);
			break;
		   case 0x0F:
		    control_Motor(120,0,245,1);
			break;
		   case 0x1E:
		    control_Motor(245,1,120,0);
			break;						
			default:
			control_Motor(190,1,190,1);
			break;
		}
	}
}
