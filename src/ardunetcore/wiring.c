extern "C" {
    #include "ardunetcore/wiring.h"
}

void init(void) {
    
}

void pinMode(uint8_t pin, uint8_t mode) {
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2); // TODO
    if (mode) {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, 1<<pin); // GPIO input
    } else {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, 1<<pin); // GPIO output
    }
}

void digitalWrite(uint8_t pin, uint8_t state) {
    if (state) {
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1<<pin); // GPIO high
    } else {
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1<<pin); // GPIO low
    }
}

int digitalRead(uint8_t pin) {
    return (GPIO_REG_READ(GPIO_OUT_ADDRESS)>>pin) & 1;
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
    vTaskDelay(ms / portTICK_RATE_MS);
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

void attachInterrupt(uint8_t, void (*)(void), int mode) {
    
}

void detachInterrupt(uint8_t) {
    
}