#include "tm4c123gh6pm.h"

/*
 * 6x 16/32-bit General Purpose Timers
 * 6x 32/64-bit Wide General Purpose Timers
 * Each Timer has 2 blocks: A and B
 */
void enable_timer1a_1000ms(void) {
    // enable clock on timer 1A
    SYSCTL_RCGCTIMER_R |= (1<<1);

    // disable timer 1 to initialize it
    TIMER1_CTL_R = 0;

    // set timer to 16-bit individual mode (page 726)
    TIMER1_CFG_R = 0x4;

    // set timer to periodic down counter interrupt trigger (page 732)
    TIMER1_TAMR_R = 0x2;

    /*
     * 16-bit timer can generate:
     * 2^16 = 65536
     * 16MHz clock = 65536 / 16*10^6 = 4.096ms
     * Prescaler add additional bits to the size of the timer
     *  8-bit and allows to reduce frequency of 16Mhz by 1-255.
     *  
     *  Prescaler is calculated via:
     *  DELAY_WANTED / MAX_DELAY (4.096ms) = PRESCALER
     */
    TIMER1_TAPR_R = 250 - 1; // 1000ms/4.096 = 250
    TIMER1_TAILR_R = 64000 - 1; // 16*10^6/250 = 64000 Hz

    // clear interrupt flag ideally via MICR register but not availabl
    TIMER1_ICR_R = 0x1; // 0x024 offset, page 754

    // enable timer module
    TIMER1_CTL_R |= 1;

    /*enables TimerA time-out  interrupt mask */
    TIMER1_IMR_R |= (1<<0);

    /*
     * Timer 1A Interrupt Request is 21
     * We can access that number via NVIC_EN0 (page 134)
     */
    NVIC_EN0_R |= (1<<21);
}
