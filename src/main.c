
//main.c
//Toggles the Red LED of TM4C Launchpad when SW1(PF4) is pushed and held
//PF4 is negative logic,i.e 0 is on and non-zero is off
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "demo.h"
#include "timer.h"

// specific port definitions
#define PF4 (*((volatile unsigned long *)0x40025040)) // SW1
#define PF3 (*((volatile unsigned long *)0x40025020)) // PF3 LED
#define PF2 (*((volatile unsigned long *)0x40025010)) // BLUE LED
#define PF1 (*((volatile unsigned long *)0x40025008)) // RED LED
#define PF0 (*((volatile unsigned long *)0x40025004)) // SW2

static unsigned long led;
static color_t blue_clr = blue;
static volatile unsigned long systime = 0; // ms timer

int main(void){
    // init port
    PortF_Init();
    
    // enable timer
    //enable_timer1a_1000ms();
    systick_enable(1); // 1ms systick like in FreeRTOS

    while(1) {
        if (!(systime%2000)) {
            toggle_portf_led(blue_clr);
        } // trigger blue led every 2s using systick
    }
}

/*
 * Interrupt Handlers: name must match the startup.c file
 */
void Timer1A_ISR(void) {
    if (TIMER1_MIS_R & 0x1) {
        toggle_portf_led(blue_clr);
        TIMER1_ICR_R = 0x1; // clear interrupt flag
    }
}

void SysTick_Handler(void) {
    systime++;
}
