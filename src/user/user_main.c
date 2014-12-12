extern "C" {
    #include "esp_common.h"
    
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}

#include "ardunetcore/ardunetcore.h"

//// SKETCH BEGIN ////

void setup() {
    Serial.begin(115200);
    
    Wifi.setMode(WIFI_MODE_HOST);
    Wifi.beginHost("ArduNet", "", 1, WIFI_AUTH_OPEN); // SSID, password, channel, Auth_method
    Wifi.setHostIP("192.168.0.42", "192.168.0.42", "255.255.255.0"); // ip, gateway, netmask

    HTTPServer.begin(80);

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

void main_task(void *pvParameters) {
    init();
    setup();
    for (;;) {
        loop();
    }
}

extern "C" void ICACHE_FLASH_ATTR user_init(void) {
    xTaskCreate(main_task, (const signed char*)"loop", 256, NULL, 2, NULL);
}

