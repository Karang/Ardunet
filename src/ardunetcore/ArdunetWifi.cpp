
#include "ardunetcore/ArdunetWifi.h"

extern "C" {
    #include "nat_libs.h"
    #include "osapi.h"
    #include "ip_addr.h"
}

ArdunetWifi::ArdunetWifi() {
    
}

void ArdunetWifi::scan(scan_done_cb_t f) {
    wifi_station_scan((scan_config*)NULL, f);
}

void ArdunetWifi::setMode(int mode) {
    if (mode<1 || mode>3) return;
    ETS_UART_INTR_DISABLE();
    wifi_set_opmode(mode);
    ETS_UART_INTR_ENABLE();
}

int ArdunetWifi::getMode() {
    return wifi_get_opmode();
}

void ArdunetWifi::beginHost(const char*ssid, const char*pwd, int channel, AUTH_MODE authMode) {
    struct softap_config apConfig;
    wifi_softap_get_config(&apConfig);

    os_strcpy(apConfig.ssid, ssid, 32);
    os_strcpy(apConfig.password, pwd, 64);
    if (channel<1 || channel>13) return;
    apConfig.channel = channel;
    if (authMode<0 || authMode>5) return;
    apConfig.authmode = authMode;

    ETS_UART_INTR_DISABLE();
    wifi_softap_set_config(&apConfig);
    ETS_UART_INTR_ENABLE();
}

void ArdunetWifi::setDeviceIP(const char*ip) {
    if (getMode()==WIFI_MODE_HOST) return;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_DEVICE, &pTempIp);
    
    pTempIp.ip.addr = ipaddr_addr(ip);
    wifi_set_ip_info(WIFI_IP_DEVICE, &pTempIp);
}

void ArdunetWifi::setHostIP(const char*ip) {
    if (getMode()==WIFI_MODE_DEVICE) return;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_HOST, &pTempIp);
    
    pTempIp.ip.addr = ipaddr_addr(ip);
    wifi_set_ip_info(WIFI_IP_HOST, &pTempIp);
}

char *ArdunetWifi::getDeviceIP() {
    char *ipStr;
    if (getMode()==WIFI_MODE_HOST) return ipStr;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_DEVICE, &pTempIp);
    os_sprintf(ipStr, "%d.%d.%d.%d", IP2STR(&pTempIp.ip));
    return ipStr;
}

char *ArdunetWifi::getHostIP() {
    char *ipStr;
    if (getMode()==WIFI_MODE_DEVICE) return ipStr;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_HOST, &pTempIp);
    os_sprintf(ipStr, "%d.%d.%d.%d", IP2STR(&pTempIp.ip));
    return ipStr;
}

void ArdunetWifi::connectToHost(const char*ssid, const char*pwd) {
    struct station_config stationConfig;

    os_bzero(&stationConfig, sizeof(struct station_config));

    os_strcpy(stationConfig.ssid, ssid, 32);
    os_strcpy(stationConfig.password, pwd, 64);

    wifi_station_disconnect();
    ETS_UART_INTR_DISABLE();
    wifi_station_set_config(&stationConfig);
    ETS_UART_INTR_ENABLE();
    wifi_station_connect();
}

void ArdunetWifi::waitIP() {
    while (wifi_station_get_connect_status()!=STATION_GOT_IP) {
        os_delay_us(10);
    }
}

ArdunetWifi Wifi = ArdunetWifi();