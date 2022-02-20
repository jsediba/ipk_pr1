#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>

#define _GNU_SOURCE
#define BAD_REQ "HTTP/1.1 400 Bad Request"
#define HEADER "HTTP/1.1 200 OK\r\nContent-Type:text/plain;\r\n\r\n"

#define HOSTNAME_STR "GET /hostname HTTP/1.1"
#define CPU_NAME_STR "GET /cpu-name HTTP/1.1"
#define LOAD_STR "GET /load HTTP/1.1"

typedef enum requst_e 
{
    UNKNOWN,
    HOSTNAME,
    CPU_NAME,
    LOAD
} request_t;