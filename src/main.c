
//main.c
//Toggles the Red LED of TM4C Launchpad when SW1(PF4) is pushed and held
//PF4 is negative logic,i.e 0 is on and non-zero is off
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"

// specific port definitions
#define PF4 (*((volatile unsigned long *)0x40025040))
#define PF3 (*((volatile unsigned long *)0x40025020))
#define PF2 (*((volatile unsigned long *)0x40025010))
#define PF1 (*((volatile unsigned long *)0x40025008))
#define PF0 (*((volatile unsigned long *)0x40025004))

unsigned long Led;

void Delay(void){
    unsigned long volatile time = 800000;
    while(time) {
      	time--;
    }
}
int main(void){
    PortF_Init();
    while(1) {
      Led = 0x06;
      GPIO_PORTF_DATA_R = Led;
      Delay();
      Led = 0x00;
      GPIO_PORTF_DATA_R = Led;
      Delay();
    }
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06
