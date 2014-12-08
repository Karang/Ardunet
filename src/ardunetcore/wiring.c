extern "C" {
    #include "nat_libs.h"
    #include "user_interface.h"
    #include "osapi.h"
    #include "gpio.h"
    #include "ardunetcore/wiring.h"
}

void init(void) {
    gpio_init();
}

void pinMode(uint8_t pin, uint8_t mode) {
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
}

void digitalWrite(uint8_t pin, uint8_t state) {
    uint8_t mask = bit(pin);
    if (state&0x1) {
        gpio_output_set(0, mask, mask, 0);
    } else {
        gpio_output_set(mask, 0, mask, 0);
    }
}

int digitalRead(uint8_t pin) {
    return GPIO_INPUT_GET(pin);
}

int analogRead(uint8_t pin) {
    return 0;
}

void analogReference(uint8_t mode) {
    
}

void analogWrite(uint8_t pin, int value) {
    
}

unsigned long millis(void) {
    return system_get_time()/1000L;
}

unsigned long micros(void) {
    return system_get_time();
}

void delay(unsigned long ms) {
    delayMicroseconds(ms*1000);
}

void delayMicroseconds(unsigned long us) {
    os_delay_us(us);
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {
    
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    
}

void attachInterrupt(uint8_t pin, void (*f)(void), int mode) {
    
}

void detachInterrupt(uint8_t pin) {
    gpio_pin_intr_state_set(GPIO_ID_PIN(pin), GPIO_PIN_INTR_DISABLE);
}