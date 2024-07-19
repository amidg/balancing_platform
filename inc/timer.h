#include "tm4c123gh6pm.h"

/*
 * SysTick setup function
 * Background time tick with specified period
 */
void systick_enable(int ms) {
    /* 
     * program value of the STRELOAD register
     * (time S) / (1/16MHz) - 1
     * 1 ms = 0.001s / (1/16*10^6) = 16000 - 1 = 15999
     */
    NVIC_ST_RELOAD_R = 16000*ms-1;

    /* clear STCURRENT value */
    NVIC_ST_CURRENT_R = 0;

    /* configure STCTRL register for the required operation mode */
    NVIC_ST_CTRL_R = 7; // see page 138 of the MCU manual
}

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
     *  )
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

/* 
 * this allows to use in-built PWM module to generate PWM signal
 * PWM is generated with specified duty cycle
 */
void enable_pwm_pf2(void) { // page 1240
    /* enable PWM via gating control using RCGCPWM register */
    SYSCTL_RCGCPWM_R = (1<<1); // page 354, set bit 1 to be 1
    
    /* enable clock divisor, page 255 */
    //SYSCTL_RCC_R |= (1<<20); // enable PWM clock div
    SYSCTL_RCC_R &= ~(1<<20); // use direct clock from the system

    /* disable PWM before configuring */
    PWM1_CTL_R &= ~(1<<0); // set bit 0

    /* select counter mode */
    PWM1_CTL_R &= ~(1<<1); // set bit 1

    /* Set PWM output when counter reloaded and clear when matches PWMCMPA */
    PWM1_3_GENA_R = 0x0000008C;

    /* set load value for 1kHz (16MHz/16000) */
    PWM1_3_LOAD_R = 16000;

    /* set duty cyle to 50% by loading of 16000 to PWM1CMPA */
    PWM1_3_CMPA_R = 8000-1;

    /* Enable Generator 3 counter */
    PWM1_3_CTL_R = 1;

    /* Enable PWM1 channel 6 output, page 1247 */
    PWM1_ENABLE_R = (1<<7);
}

// duty must be between 0 and 1
void set_pf2_pwm_duty(float duty) {
    PWM1_3_CMPA_R = 16000*duty - 1;
}
