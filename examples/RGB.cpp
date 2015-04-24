WS2812 strip;

void setup() {
  	strip.begin(2, 12);
}

void loop() {
  strip.setRGB(3, 10, 0, 0);
  strip.sync();
  delay(1000);
  strip.setRGB(3, 0, 10, 0);
  strip.sync();
  delay(1000);
  strip.setRGB(3, 0, 0, 10);
  strip.sync();
  delay(1000);
}