#ifndef __ARDUNET_HTTP_CLIENT_H__
#define __ARDUNET_HTTP_CLIENT_H__

extern "C" {
    #include "esp_common.h"
}

class ArdunetHTTPClient {
    private:
    public:
        ArdunetHTTPClient();
        void setProxy(char*url, int port);
        void get(char*url);
        void post(char*url);
};

extern ArdunetHTTPClient HTTPClient;

#endif