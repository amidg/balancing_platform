
//main.c
//Toggles the Red LED of TM4C Launchpad when SW1(PF4) is pushed and held
//PF4 is negative logic,i.e 0 is on and non-zero is off
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "demo.h"

// specific port definitions
#define PF4 (*((volatile unsigned long *)0x40025040))
#define PF3 (*((volatile unsigned long *)0x40025020))
#define PF2 (*((volatile unsigned long *)0x40025010))
#define PF1 (*((volatile unsigned long *)0x40025008))
#define PF0 (*((volatile unsigned long *)0x40025004))

static unsigned long led;

int main(void){
    // init port
    PortF_Init();

    // colors
    color_t blue_color = blue;
    color_t no_color = none;

    while(1) {
       set_portf_led(blue_color);
       delay();
       set_portf_led(no_color);
       delay();
    }
}
