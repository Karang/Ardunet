char* homeHandler(HeaderInfo* infos) {
    const char* content = "<h1>Ardunet test</h1><p>Change state of the led :<br/><a href=\"/high\">High</a><br/><a href=\"/low\">Low</a></p>";
    return (char*)content;
}

char* ledHighHandler(HeaderInfo* infos) {
    digitalWrite(12, HIGH);
    const char* content = "<h1>Ardunet LED</h1><p>Led state = high<br/><a href=\"/\">Home</a></p>";
    return (char*)content;
}

char* ledLowHandler(HeaderInfo* infos) {
    digitalWrite(12, LOW);
    const char* content = "<h1>Ardunet LED</h1><p>Led state = low<br/><a href=\"/\">Home</a></p>";
    return (char*)content;
}

void setup() {
    Serial.begin(115200);
    
    Wifi.setMode(WIFI_MODE_HOST);
    Wifi.beginHost("ArduNet", "", 1, WIFI_AUTH_OPEN); // ssid, password, channel, auth_method
    Wifi.setHostIP("192.168.0.42", "192.168.0.42", "255.255.255.0"); // ip, gateway, netmask

    HTTPServer.begin(80);
    HTTPServer.registerURL("/", homeHandler);
    HTTPServer.registerURL("/high", ledHighHandler);
    HTTPServer.registerURL("/low", ledLowHandler);

    pinMode(12, OUTPUT);
}

void loop() {
    delay(1000);
}