void setup() {
    pinMode(2, OUTPUT);
}

void loop() {
    for (int i=0 ; i<=255 ; i++) {
        analogWrite(2, i);
        delay(5);
    }
    delay(100);
    for (int i=255 ; i>=0 ; i--) {
        analogWrite(2, i);
        delay(5);
    }
    delay(100);
}