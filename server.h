#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

#define _GNU_SOURCE

#define BUFFER_SIZE 1024

#define BAD_REQ "HTTP/1.1 400 Bad Request\r\n"
#define NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define HEADER "HTTP/1.1 200 OK\r\nContent-Type:text/plain;\r\n\r\n"

#define HOSTNAME_STR "GET /hostname HTTP/1.1"
#define CPU_NAME_STR "GET /cpu-name HTTP/1.1"
#define LOAD_STR "GET /load HTTP/1.1"
#define FAVICON_STR "GET /favicon.ico HTTP/1.1"

typedef struct socket_info{
    int socket;
    int opt;
    struct sockaddr_in addr;
} socket_info_t;

void get_hostname(int c_socket);
void get_cpu_name(int c_socket);
void get_cpu_load(int c_socket);
void parse_http_request(char *msg, int c_socket);
unsigned short parse_args(int argc, char **argv);
socket_info_t prepare_socket(unsigned short port_num);