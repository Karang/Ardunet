#ifndef __ARDUNET_HTTP_SERVER_H__
#define __ARDUNET_HTTP_SERVER_H__

extern "C" {
    #include "esp_common.h"
    
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    
    #include "lwip/sockets.h"
    #include "lwip/dns.h"
    #include "lwip/netdb.h"
}

#define GET 0
#define POST 1

typedef struct {
    const char*url;
    int type;
} HeaderInfo;

class ArdunetHTTPServer {
    private:
        
    public:
        ArdunetHTTPServer();
        void begin(uint8_t p);
        
        void registerURL(const char*, char*(*)(HeaderInfo*));
        uint8_t port;
};

extern ArdunetHTTPServer HTTPServer;

#endif