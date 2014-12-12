#include "ardunetcore/ArdunetHTTPServer.h"

ArdunetHTTPServer::ArdunetHTTPServer() {
    
}

static const char *httpNotFoundHeader="HTTP/1.0 404 Not Found\r\nServer: esp8266-ArduNet/0.1\r\nContent-Type: text/html\r\n\r\n<h1>Ardunet</h1>Sorry, content not found :(\r\n";

void server_task(void *pvParameters) {
    
    ArdunetHTTPServer* server = (ArdunetHTTPServer*) pvParameters;
    
    while (1) {
        struct sockaddr_in server_addr, client_addr;
        int server_sock, client_sock;
        socklen_t sin_size;
        bzero(&server_addr, sizeof(struct sockaddr_in));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(server->port);
        int recbytes;
        do {
            if (-1 == (server_sock = socket(AF_INET, SOCK_STREAM, 0))) {
                printf("S > socket error\n");
                break;
            }
            printf("S > create socket: %d\n", server_sock);
            if (-1 == bind(server_sock, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr))) {
                printf("S > bind fail\n");
                break;
            }
            printf("S > bind port: %d\n", ntohs(server_addr.sin_port));
            if (-1 == listen(server_sock, 5)) {
                printf("S > listen fail\n");
                break;
            }
            printf("S > listen ok\n");
            sin_size = sizeof(client_addr);
            for (;;) {
                printf("S > wait client\n");
                if ((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &sin_size)) < 0) {
                    printf("S > accept fail\n");
                    continue;
                }
                printf("S > Client from %s %d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
                char *recv_buf = (char *)zalloc(128);
                while ((recbytes = read(client_sock , recv_buf, 128)) > 0) {
                    recv_buf[recbytes] = 0;
                    printf("S > read data success %d!\nS > %s\n", recbytes, recv_buf);
                    if (recbytes < 128) {
                        if (write(client_sock, httpNotFoundHeader, strlen(httpNotFoundHeader)+1) < 0) {
                            printf("S > Send fail!\n");
                        }
                        printf("S > Send success!\n");
                        close(client_sock);
                    }
                }
                free(recv_buf);
                
                if (recbytes <= 0) {
                    printf("S > read data fail!\n");
                    close(client_sock);
                }
            }
        } while (0);
    }
}

void ArdunetHTTPServer::begin(uint8_t p) {
    port = p;
    xTaskCreate(server_task, (const signed char*)"http_server", 256, this, 2, NULL);
}

void ArdunetHTTPServer::registerURL(const char*url, void(*callback)()) {
    
}

ArdunetHTTPServer HTTPServer = ArdunetHTTPServer();