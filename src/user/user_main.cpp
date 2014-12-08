extern "C" {
  #include "ets_sys.h"
  #include "user_interface.h"
  #include "nat_libs.h"
  #include "osapi.h"
  #include "driver/uart.h"
  #include "gpio.h"
  #include "os_type.h"
  #include "user_config.h"
}

#include "ardunetcore/ardunetcore.h"

//// SKETCH BEGIN ////

void setup() {
    Serial.begin(115200);
    
    Wifi.setMode(WIFI_MODE_HOST_DEVICE);
    
    Wifi.beginHost("ArduNet", "", 1, WIFI_AUTH_OPEN);
    Wifi.setHostIP("192.168.0.42");
    //Wifi.setDeviceIP("192.168.0.42");
    
    WebServer.begin(80);
    
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}

void loop() {
    Serial.println(millis());
    //Serial.println(digitalRead(2));
    
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
}

//// SKETCH END ////

#define LOOP_PRIO 0
#define SIG_LOOP 1
#define LOOP_QUEUE_LEN 1

os_event_t loop_task_queue[LOOP_QUEUE_LEN];

static void ICACHE_FLASH_ATTR loop_func(os_event_t *events) {
    if (events->sig == SIG_LOOP) {
        loop();
        system_os_post(LOOP_PRIO, SIG_LOOP, 0);
    }
}

extern "C" void user_init(void) {
    init();
    setup();
    
    //system_os_task(loop_func, LOOP_PRIO, loop_task_queue, LOOP_QUEUE_LEN);
    //system_os_post(LOOP_PRIO, SIG_LOOP, 0);
}
