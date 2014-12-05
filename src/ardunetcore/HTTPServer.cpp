#include "ardunetcore/HTTPServer.h"

static void http_client_discon_cb(void *arg) {

}

static const char *httpNotFoundHeader="HTTP/1.0 404 Not Found\r\nServer: esp8266-ArduNet/0.1\r\nContent-Type: text/plain\r\n\r\nNot Found.\r\n";

static void http_client_recv_cb(void *arg, char *pdata, unsigned short len) {
    struct espconn *conn = (struct espconn *)arg;
    espconn_sent(conn, (uint8_t *)httpNotFoundHeader, os_strlen(httpNotFoundHeader));
}

static void http_client_sent_cb(void *arg) {

}

static void http_client_connect_cb(void *arg) {
    struct espconn *conn = (struct espconn *)arg;

    espconn_regist_disconcb(conn, http_client_discon_cb);
    espconn_regist_recvcb(conn, http_client_recv_cb);
    espconn_regist_sentcb(conn, http_client_sent_cb);
}

HTTPServer::HTTPServer() {
    
}

void HTTPServer::begin(uint8_t port) {
    httpdConn.type = ESPCONN_TCP;
	httpdConn.state = ESPCONN_NONE;
	httpdTcp.local_port = port;
	httpdConn.proto.tcp = &httpdTcp;

	espconn_regist_connectcb(&httpdConn, http_client_connect_cb);
	espconn_accept(&httpdConn);
}

void HTTPServer::startHeader(struct espconn *conn, int code) {
    char buff[128];
    int l = os_sprintf(buff, "HTTP/1.0 %d OK\r\nServer: esp8266-ArduNet/0.1\r\n", code);
    espconn_sent(conn, (uint8_t *)buff, l);
}

void HTTPServer::sendHeader(struct espconn *conn, const char*field, const char*val) {
    char buff[256];
    int l = os_sprintf(buff, "%s: %s\r\n", field, val);
    espconn_sent(conn, (uint8 *)buff, l);
}

void HTTPServer::endHeader(struct espconn *conn) {
    espconn_sent(conn, (uint8_t *)"\r\n", 2);
}

HTTPServer WebServer = HTTPServer();