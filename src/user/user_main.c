extern "C" {
    #include "esp_common.h"
    
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    
    #include "lwip/sockets.h"
    #include "lwip/dns.h"
    #include "lwip/netdb.h"
}

#include "ardunetcore/ardunetcore.h"

//// SKETCH BEGIN ////

void setup() {
    Serial.begin(115200);
    
    Wifi.setMode(WIFI_MODE_HOST_DEVICE);
    Wifi.beginHost("ArduNet", "", 1, WIFI_AUTH_OPEN);
    Wifi.setHostIP("192.168.0.42", "192.168.0.42", "255.255.255.0");

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    pinMode(2, OUTPUT);
}

void loop() {
    Serial.print("Hello world !\n\r");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
}

//// SKETCH END ////

void task2(void *pvParameters) {
    init();
    setup();
    for (;;) {
        loop();
    }
}

extern "C" void ICACHE_FLASH_ATTR user_init(void) {
    xTaskCreate(task2, (const signed char*)"tsk2", 256, NULL, 2, NULL);
}

