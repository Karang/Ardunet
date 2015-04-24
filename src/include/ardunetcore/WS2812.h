#ifndef __WS2812_H__
#define __WS2812_H__

extern "C" {
    #include "esp_common.h"
    #include "ardunetcore/wiring.h"
}

class WS2812 {
    public: 
    	WS2812();
    	void begin(uint8_t pin, uint16_t num_led);
    	
        void setRGB(uint16_t led, uint8_t r, uint8_t g, uint8_t b);
    	void sync();
    private:
        void ws2812_send_one(uint8_t gpio);
        void ws2812_send_zero(uint8_t gpio);
        
        uint16_t count_led;
    	uint8_t ws_pin;
    	uint8_t pixels[36];
};

#endif