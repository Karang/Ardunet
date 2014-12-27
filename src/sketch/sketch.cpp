#include "ardunetcore/ardunetcore.h"

#line 1
void setup() {
    pinMode(12, OUTPUT);
}

void loop() {
    for (int i=0 ; i<=255 ; i++) {
        analogWrite(12, i);
        delay(5);
    }
    delay(100);
    for (int i=255 ; i>=0 ; i--) {
        analogWrite(12, i);
        delay(5);
    }
    delay(100);
}