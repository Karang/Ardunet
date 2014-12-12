#ifndef __ARDUNET_WIFI_H__
#define __ARDUNET_WIFI_H__

extern "C" {
    #include "esp_common.h"
    #include "udhcp/dhcpd.h"
    #include "freertos/portmacro.h"
}

#define WIFI_MODE_NULL          NULL_MODE
#define WIFI_MODE_DEVICE        STATION_MODE
#define WIFI_MODE_HOST          SOFTAP_MODE
#define WIFI_MODE_HOST_DEVICE   STATIONAP_MODE

#define WIFI_AUTH_OPEN          AUTH_OPEN
#define WIFI_AUTH_WEP           AUTH_WEP
#define WIFI_AUTH_WPA_PSK       AUTH_WPA_PSK
#define WIFI_AUTH_WPA2_PSK      AUTH_WPA2_PSK
#define WIFI_AUTH_WPA_WPA2_PSK  AUTH_WPA_WPA2_PSK

#define WIFI_IP_DEVICE          0x00
#define WIFI_IP_HOST            0x01

class ArdunetWifi {
    private:
    public:
        ArdunetWifi();
        void scan(scan_done_cb_t);
        void setMode(int wifiMode);
        int getMode();
        void beginHost(const char*ssid, const char*pwd, int channel, AUTH_MODE auth);
        void setDeviceIP(const char*ip);
        void setHostIP(const char*ip);
        void setHostIP(const char*ip, const char*gateway, const char*netmask);
        char *getDeviceIP();
        char *getHostIP();
        void connectToHost(const char*ssid, const char*pwd);
        void disconnectFromHost();
        void waitIP();
};

extern ArdunetWifi Wifi;

#endif