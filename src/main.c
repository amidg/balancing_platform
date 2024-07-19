
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
static uint8_t duty = 10; // 10%, int value is used because of floating point building bug, need to figure it out
static uint8_t direction = 1; // rising direction

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

    /* use PWM and systick to control PF2 blue led brightness */
    enable_pwm_portf();
    enable_pwm_pf2(); // start PWM hardware with 50% duty
    set_pf2_pwm_duty(duty);
    while (1) {
        // control LED brightness by changing duty cycle
        if (!(systime%250)) {
            if (duty == 100)
                direction = 0;
            else if (duty == 10)
                direction = 1;
            set_pf2_pwm_duty(duty);
            duty = (direction) ? duty+10 : duty-10;
        }
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
