#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

extern "C" {
    #include "nat_libs.h"
    #include "user_interface.h"
    #include "espconn.h"
    #include "osapi.h"
    
    typedef struct {
        const char* ext;
        const char* mimeType;
    } MimeMap;
    
    static const MimeMap mimeTypes[] = {
    	{"htm", "text/htm"},
    	{"html", "text/html"},
    	{"js", "text/javascript"},
    	{"txt", "text/plain"},
    	{"jpg", "image/jpeg"},
    	{"jpeg", "image/jpeg"},
    	{"png", "image/png"},
    	{"bmp", "image/bmp"},
    	{"NULL", "text/html"}, //default value
    };
}

class HTTPServer {
    private:
        struct espconn httpdConn;
        esp_tcp httpdTcp;
    public:
        HTTPServer();
        void begin(uint8_t port);
        char* getMimeType(char*url);
        void startHeader(struct espconn *conn, int code);
        void sendHeader(struct espconn *conn, const char*field, const char*val);
        void endHeader(struct espconn *conn);
};

extern HTTPServer WebServer;

#endif