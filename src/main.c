
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
static float duty_cycle = 0.1;

int main(void){
    // init port
    PortF_Init();
    
    /* use 1 second hardware timer for blinking
    enable_timer1a_1000ms();
    while(1);
    */
    
    // 1ms systick like in FreeRTOS
    systick_enable(1); 

    /* use systick timer with specified 1ms tick to toggle LED
    while(1) {
        if (!(systime%500)) {
            toggle_portf_led(blue_clr);
        }
    } // */

    // use PWM and systick to control PF2 (blue) brightness
    // add PWM to the PF2
    GPIO_PORTF_AFSEL_R = (1<<2); // page 671, enable alt func
    GPIO_PORTF_PCTL_R |= 0x00000500; // add PWM to PF2, page 688 & 1351
    enable_pwm_pf2(); // start PWM hardware with 50% duty
    set_pf2_pwm_duty(0.5);
    int iter = 0, direction = 1;
    while (1) {
        // control LED brightness by changing duty cycle
        //if (!(systime%100)) { // trigger every 100ms
        //    if (direction) {
        //        ++iter;
        //        direction = !(iter == 1);
        //    }
        //    else {
        //        --iter;
        //        direction = (iter == 0.1);
        //    }
        //        --iter;
        //    // apply PWM
        //    set_pf2_pwm_duty((float)duty_cycle*iter);
        //}
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
