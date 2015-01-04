int count = 0;

void isr_count() {
    count++;
}

void setup() {
    Serial.begin(115200);
    pinMode(2, INPUT);
    attachInterrupt(2, isr_count, FALLING);
}

void loop() {
    Serial.println(count);
    count = 0;
    delay(1000);
}