void setup() {
    Serial.begin(115200);
    Wifi.setMode(WIFI_MODE_DEVICE);
}

void Wifi_scan_found_bss(uint8* bssid, int channel, int rssi, int authmode, char* ssid, int is_hidden) {
    Serial.println("---------------------");
    Serial.print("BSSID : ");
    Serial.printBSSID(bssid);
    Serial.println();
    Serial.print("Channel : ");
    Serial.println(channel);
    Serial.print("RSSI : ");
    Serial.println(rssi);
    Serial.print("Auth : ");
    Serial.println(authmode);
    Serial.print("SSID : ");
    Serial.println(ssid);
    Serial.print("Is hidden : ");
    Serial.println(is_hidden);
}

void loop() {
    Serial.println("Start scan...");
    Wifi.scan();
    delay(10000);
}