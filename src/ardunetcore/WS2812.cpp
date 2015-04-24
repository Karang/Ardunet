#include "ardunetcore/WS2812.h"
#include "ardunetcore/HardwareSerial.h"

WS2812::WS2812() {
    
}

void WS2812::begin(uint8_t pin, uint16_t num_led) {
    count_led = num_led;
    ws_pin = pin;
    //pixels = (uint8_t*)malloc(count_led*3);
}

void WS2812::setRGB(uint16_t led, uint8_t r, uint8_t g, uint8_t b) {
    pixels[led*3] = g;
    pixels[led*3+1] = r;
    pixels[led*3+2] = b;
    Serial.println(pixels[led*3], DEC);
}

void WS2812::ws2812_send_one(uint8_t gpio) {
    uint8_t i;
    i = 8; while (i--) GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1 << gpio);
    i = 5; while (i--) GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1 << gpio);
}

void WS2812::ws2812_send_zero(uint8_t gpio) {
    uint8_t i;
    i = 1; while (i--) GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1 << gpio);
    i = 12; while (i--) GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1 << gpio);
}

void WS2812::sync() {
    pinMode(ws_pin, OUTPUT);
    noInterrupts();
	for (uint8_t i=0 ; i < (count_led*3) ; i++) {
		uint8_t mask = 0x80;
        while (mask) {
            if (pixels[i] & mask) ws2812_send_one(ws_pin);
            else ws2812_send_zero(ws_pin);
            mask >>= 1;
        }
	}
	delayMicroseconds(50);
	interrupts();
}