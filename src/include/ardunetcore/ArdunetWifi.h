#ifndef __ARDUNET_WIFI_H__
#define __ARDUNET_WIFI_H__

extern "C" {
    #include "user_interface.h"
}

#define WIFI_MODE_NULL          0x00
#define WIFI_MODE_DEVICE        0x01
#define WIFI_MODE_HOST          0x02
#define WIFI_MODE_HOST_DEVICE   0x03

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
        char *getDeviceIP();
        char *getHostIP();
        void connectToHost(const char*ssid, const char*pwd);
        void waitIP();
};

extern ArdunetWifi Wifi;

#endif