extern "C" {
    #include "ardunetcore/wiring.h"
    #include "ardunetcore/pwm.h"
}

#include "ardunetcore/HardwareSerial.h"

int gpio_pin_register[16] = {PERIPHS_IO_MUX_GPIO0_U,
                             PERIPHS_IO_MUX_U0TXD_U,
                             PERIPHS_IO_MUX_GPIO2_U,
                             PERIPHS_IO_MUX_U0RXD_U,
                             PERIPHS_IO_MUX_GPIO4_U,
                             PERIPHS_IO_MUX_GPIO5_U,
                             PERIPHS_IO_MUX_SD_CLK_U,
                             PERIPHS_IO_MUX_SD_DATA0_U,
                             PERIPHS_IO_MUX_SD_DATA1_U,
                             PERIPHS_IO_MUX_SD_DATA2_U,
                             PERIPHS_IO_MUX_SD_DATA3_U,
                             PERIPHS_IO_MUX_SD_CMD_U,
                             PERIPHS_IO_MUX_MTDI_U,
                             PERIPHS_IO_MUX_MTCK_U,
                             PERIPHS_IO_MUX_MTMS_U,
                             PERIPHS_IO_MUX_MTDO_U};

#define GPIO_PIN_ADDR(i)    (GPIO_PIN0_ADDRESS + i*4)

LOCAL void (*callbacks[16])(void);

LOCAL void interruptHandler() {
    uint32_t gpio_mask = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    for (int i=0 ; i<16 ; i++) {
        if ((0x1<<i) & gpio_mask) {
            if (callbacks[i] != NULL) {
                (*callbacks[i])();
            }
        }
    }
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_mask);
}

void interrupts() {
    portEXIT_CRITICAL();
    #define ETS_GPIO_INUM       4
    _xt_isr_unmask(1<<ETS_GPIO_INUM);
    _xt_isr_unmask(1<<ETS_UART_INUM);
    _xt_isr_unmask(1<<ETS_MAX_INUM);
    _xt_isr_unmask(1<<ETS_SOFT_INUM);
    _xt_isr_unmask(1<<ETS_WDT_INUM);
    _xt_isr_unmask(1<<ETS_FRC_TIMER1_INUM);
}
void noInterrupts() {
    portENTER_CRITICAL();
    _xt_isr_mask(1<<ETS_GPIO_INUM);
    _xt_isr_mask(1<<ETS_UART_INUM);
    _xt_isr_mask(1<<ETS_MAX_INUM);
    _xt_isr_mask(1<<ETS_SOFT_INUM);
    _xt_isr_mask(1<<ETS_WDT_INUM);
    _xt_isr_mask(1<<ETS_FRC_TIMER1_INUM);
}

void ICACHE_FLASH_ATTR init(void) {
    for (int i=0 ; i<16 ; i++) {
        detachInterrupt(i);
    }
    _xt_isr_attach(ETS_GPIO_INUM, (_xt_isr)interruptHandler);
    _xt_isr_unmask(1<<ETS_GPIO_INUM);
    
    //uint8_t duty[PWM_CHANNEL] = {0,0,0};
    //pwm_init(100, duty);
}

void ICACHE_FLASH_ATTR pinMode(uint8_t pin, uint8_t mode) {
    if ((0x1 << pin) & 0b110101) {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 0);
    } else {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 3);
    }
    PIN_PULLDWN_DIS(gpio_pin_register[pin]);
    PIN_PULLUP_EN(gpio_pin_register[pin]);
    if (mode) {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, 1<<pin); // GPIO input
    } else {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, 1<<pin); // GPIO output
    }
}

void ICACHE_FLASH_ATTR digitalWrite(uint8_t pin, uint8_t state) {
    if (state) {
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1<<pin); // GPIO high
    } else {
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1<<pin); // GPIO low
    }
}

int ICACHE_FLASH_ATTR digitalRead(uint8_t pin) {
    return (GPIO_REG_READ(GPIO_OUT_ADDRESS)>>pin) & 1;
}

int ICACHE_FLASH_ATTR analogRead(uint8_t pin) {
    return 0;
}

void ICACHE_FLASH_ATTR analogReference(uint8_t mode) {
    
}

void ICACHE_FLASH_ATTR analogWrite(uint8_t pin, int value) {
    pwm_set_duty(value, 0);
    pwm_start();
}

unsigned long ICACHE_FLASH_ATTR millis(void) {
    return system_get_time()/1000L;
}

unsigned long ICACHE_FLASH_ATTR micros(void) {
    return system_get_time();
}

void ICACHE_FLASH_ATTR delay(unsigned long ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void ICACHE_FLASH_ATTR delayMicroseconds(unsigned long us) {
    os_delay_us(us);
}

unsigned long ICACHE_FLASH_ATTR pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {
    
}

void ICACHE_FLASH_ATTR shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    
}

uint8_t ICACHE_FLASH_ATTR shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    
}

void ICACHE_FLASH_ATTR attachInterrupt(uint8_t pin, void (*callback)(void), int mode) {
    if (pin<0 || pin>=16) return;
    callbacks[pin] = callback;
    
    portENTER_CRITICAL();
    uint32 pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(pin));
    pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
    pin_reg |= (mode << GPIO_PIN_INT_TYPE_LSB);
    GPIO_REG_WRITE(GPIO_PIN_ADDR(pin), pin_reg);
    portEXIT_CRITICAL();
}

void ICACHE_FLASH_ATTR detachInterrupt(uint8_t pin) {
    if (pin<0 || pin>=16) return;
    callbacks[pin] = NULL;
    
    portENTER_CRITICAL();
    uint32 pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(pin));
    pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
    pin_reg |= (GPIO_PIN_INTR_DISABLE << GPIO_PIN_INT_TYPE_LSB);
    GPIO_REG_WRITE(GPIO_PIN_ADDR(pin), pin_reg);
    portEXIT_CRITICAL();
}