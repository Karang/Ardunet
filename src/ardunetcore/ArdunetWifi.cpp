
#include "ardunetcore/ArdunetWifi.h"

ICACHE_FLASH_ATTR ArdunetWifi::ArdunetWifi() {
    
}

void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status) {
	scaninfo *c = (scaninfo *) arg; 
	struct bss_info *inf;
    STAILQ_FOREACH(inf, c->pbss, next) {
	    Wifi_scan_found_bss(inf->bssid, (int)inf->channel, (int)inf->rssi, (int)inf->authmode, (char*)inf->ssid, (int)inf->is_hidden);
		inf = (struct bss_info *) &inf->next;
	}
}

void ICACHE_FLASH_ATTR ArdunetWifi::scan() {
    if (getMode()!=WIFI_MODE_DEVICE) return;
    wifi_station_scan((scan_config*)NULL, scan_done);
}

void ICACHE_FLASH_ATTR ArdunetWifi::setMode(int mode) {
    if (mode<1 || mode>3) return;
    if (mode==WIFI_MODE_HOST)
        wifi_station_disconnect();
    portENTER_CRITICAL();
    wifi_set_opmode(mode);
    portEXIT_CRITICAL();
}

int ICACHE_FLASH_ATTR ArdunetWifi::getMode() {
    return wifi_get_opmode();
}

void ICACHE_FLASH_ATTR ArdunetWifi::beginHost(const char*ssid, const char*pwd, int channel, AUTH_MODE authMode) {
    struct softap_config apConfig;
    wifi_softap_get_config(&apConfig);

    sprintf((char*)apConfig.ssid, ssid, 32);
    sprintf((char*)apConfig.password, pwd, 64);
    if (channel<1 || channel>13) return;
    apConfig.channel = channel;
    if (authMode<0 || authMode>5) return;
    apConfig.authmode = authMode;

    portENTER_CRITICAL();
    wifi_softap_set_config(&apConfig);
    portEXIT_CRITICAL();
    
    /*struct dhcp_info *pdhcp_info = NULL;
    pdhcp_info = (struct dhcp_info *)zalloc(sizeof(struct dhcp_info));
    pdhcp_info->start_ip = ipaddr_addr("192.168.0.100");
    pdhcp_info->end_ip = ipaddr_addr("192.168.0.110"); // don't set the range too large, because it will cost memory.
    pdhcp_info->max_leases = 10;
    pdhcp_info->auto_time = 60;
    pdhcp_info->decline_time = 60;
    pdhcp_info->conflict_time = 60;
    pdhcp_info->offer_time = 60;
    pdhcp_info->min_lease_sec = 60;
    dhcp_set_info(pdhcp_info);
    free(pdhcp_info);*/
    
    udhcpd_start();
}

void ICACHE_FLASH_ATTR ArdunetWifi::setDeviceIP(const char*ip) {
    if (getMode()==WIFI_MODE_HOST) return;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_DEVICE, &pTempIp);
    
    pTempIp.ip.addr = ipaddr_addr(ip);
    wifi_set_ip_info(WIFI_IP_DEVICE, &pTempIp);
}

void ICACHE_FLASH_ATTR ArdunetWifi::setHostIP(const char*ip) {
    if (getMode()==WIFI_MODE_DEVICE) return;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_HOST, &pTempIp);
    
    pTempIp.ip.addr = ipaddr_addr(ip);
    wifi_set_ip_info(WIFI_IP_HOST, &pTempIp);
}

void ICACHE_FLASH_ATTR ArdunetWifi::setHostIP(const char*ip, const char*gateway, const char*netmask) {
    if (getMode()==WIFI_MODE_DEVICE) return;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_HOST, &pTempIp);
    
    pTempIp.ip.addr = ipaddr_addr(ip);
    pTempIp.gw.addr = ipaddr_addr(gateway);
    pTempIp.netmask.addr = ipaddr_addr(netmask);
    
    wifi_set_ip_info(WIFI_IP_HOST, &pTempIp);
}

char* ICACHE_FLASH_ATTR ArdunetWifi::getDeviceIP() {
    char *ipStr;
    if (getMode()==WIFI_MODE_HOST) return ipStr;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_DEVICE, &pTempIp);
    sprintf(ipStr, "%d.%d.%d.%d", IP2STR(&pTempIp.ip));
    return ipStr;
}

char* ICACHE_FLASH_ATTR ArdunetWifi::getHostIP() {
    char *ipStr;
    if (getMode()==WIFI_MODE_DEVICE) return ipStr;
    
    struct ip_info pTempIp;
    wifi_get_ip_info(WIFI_IP_HOST, &pTempIp);
    sprintf(ipStr, "%d.%d.%d.%d", IP2STR(&pTempIp.ip));
    return ipStr;
}

void ICACHE_FLASH_ATTR ArdunetWifi::connectToHost(const char*ssid, const char*pwd) {
    struct station_config stationConfig;

    bzero(&stationConfig, sizeof(struct station_config));

    sprintf((char*)stationConfig.ssid, ssid);
    sprintf((char*)stationConfig.password, pwd);

    wifi_station_disconnect();
    portENTER_CRITICAL();
    wifi_station_set_config(&stationConfig);
    portEXIT_CRITICAL();
    wifi_station_connect();
}

void ICACHE_FLASH_ATTR ArdunetWifi::disconnectFromHost() {
    wifi_station_disconnect();
}

void ICACHE_FLASH_ATTR ArdunetWifi::waitIP() {
    while (wifi_station_get_connect_status()!=STATION_GOT_IP) {
        os_delay_us(10);
    }
}

ArdunetWifi Wifi = ArdunetWifi();