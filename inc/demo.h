#include "tm4c123gh6pm.h"

typedef enum color {
    none = 0x00,
    red = 0x02,
    blue = 0x04,
    green = 0x08,
    yellow = 0x0A,
    sky = 0x0C,
    white = 0x0E,
    pink = 0x06
} color_t;

void delay(void) {
    unsigned long volatile time = 800000; // 100ms
    while(time) {
      	time--;
    }
}

void toggle_portf_led(uint32_t value) {
    GPIO_PORTF_DATA_R ^= value;
}
