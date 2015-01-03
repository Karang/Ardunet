#include "ardunetcore/ArdunetHTTPServer.h"

static const char *httpNotFoundHeader="HTTP/1.0 404 Not Found\r\nServer: esp8266-ArduNet/0.1\r\nContent-Type: text/html\r\n\r\n<h1>Ardunet</h1>Sorry, content not found :(\r\n";
static const char *httpOkHeader="HTTP/1.0 200 OK\r\nServer: esp8266-ArduNet/0.1\r\nContent-Type: text/html\r\n\r\n";

typedef struct {
    const char*url;
    char*(*callback)(HeaderInfo*);
} URLCallback;

#define MAX_URLS    16

URLCallback urls[MAX_URLS];

ArdunetHTTPServer::ArdunetHTTPServer() {
    for (int i=0 ; i<MAX_URLS ; i++) {
        urls[i].url == NULL;
    }
}

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
                
                HeaderInfo infos;
                char *recv_buf = (char *)zalloc(512);
                int recv_index = 0;
                while ((recbytes = read(client_sock , recv_buf+recv_index, 1)) > 0) {
                    recv_index += recbytes;
                    if (recv_index>=2 && recv_buf[recv_index-2]=='\r' && recv_buf[recv_index-1]=='\n') {
                        recv_buf[recv_index-2] = 0;
                        if (strncmp(recv_buf, "GET ", 4)==0) {
                            infos.type = GET;
                            char*url;
                            //Skip past the space after POST/GET
                            int i=0;
                            while (recv_buf[i]!=' ') i++;
                            url = recv_buf+i+1;
                            //Figure out end of url.
                            char *e = (char*)strstr(url, " ");
                            if (e==NULL) return; //wtf?
                            *e = 0; //terminate url part
                            printf("URL = %s\n", url);
                            strcpy((char*)infos.url, url);
                            //TODO : parse get arguments
                        } else if (strncmp(recv_buf, "POST ", 5)==0) {
                            infos.type = POST;
                            char*url;
                            //Skip past the space after POST/GET
                            int i=0;
                            while (recv_buf[i]!=' ') i++;
                            url = recv_buf+i+1;
                            //Figure out end of url.
                            char *e = (char*)strstr(url, " ");
                            if (e==NULL) return; //wtf?
                            *e = 0; //terminate url part
                            printf("URL = %s\n", url);
                            strcpy((char*)infos.url, url);
                        }
                        if (recv_index<=2) {
                            break;
                        }
                        recv_index = 0;
                    }
                }
                free(recv_buf);
                
                int i;
                for (i=0 ; i<MAX_URLS ; i++) {
                    if (urls[i].url != NULL) {
                        printf("%s %s %d %d", urls[i].url, infos.url, strlen(urls[i].url), strlen(infos.url));
                        if (strncmp(urls[i].url, infos.url, strlen(infos.url))==0) {
                            write(client_sock, httpOkHeader, strlen(httpOkHeader)+1);
                            char* content = (*urls[i].callback)(&infos);
                            write(client_sock, content, strlen(content)+1);
                            break;
                        }
                    }
                }
                
                if (i==MAX_URLS) { // 404 Not found
                    write(client_sock, httpNotFoundHeader, strlen(httpNotFoundHeader)+1);
                }
                
                close(client_sock);
            }
        } while (0);
    }
}

void ArdunetHTTPServer::begin(uint8_t p) {
    port = p;
    xTaskCreate(server_task, (const signed char*)"http_server", 256, this, 2, NULL);
}

void ArdunetHTTPServer::registerURL(const char*url, char*(*callback)(HeaderInfo*)) {
    for (int i=0 ; i<MAX_URLS ; i++) {
        if (urls[i].url == NULL) {
            urls[i].url = url;
            urls[i].callback = callback;
            break;
        }
    }
}

ArdunetHTTPServer HTTPServer = ArdunetHTTPServer();