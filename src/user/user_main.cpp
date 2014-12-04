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
    
    Wifi.setMode(WIFI_MODE_HOST);
    Wifi.beginHost("Ardunet", "", 1, WIFI_AUTH_OPEN);
    Wifi.setHostIP("192.168.0.42");
    
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}

void loop() {
    Serial.println("Hello world !");
    Serial.print("This is pi : ");
    Serial.println(PI, 4);
    Serial.println(millis());
    
    if (digitalRead(2) == HIGH) {
        digitalWrite(2, LOW);
    } else {
        digitalWrite(2, HIGH);
    }
}

//// SKETCH END ////

static volatile os_timer_t loop_timer;

void loop_func(void *arg) {
    loop();
}

extern "C" void user_init(void) {
  init();
  setup();

  // TODO: use task instead of timer (edit uart.c accordingly)
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *)loop_func, NULL);
  os_timer_arm(&loop_timer, 1000, 1);
}
