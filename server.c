
#include "server.h"

void get_hostname(int c_socket)
{
    FILE *f = popen("hostname", "r");
    if (f == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    char buffer[BUFFER_SIZE] = {0};
    char *res = fgets(buffer, BUFFER_SIZE, f);
    if (res == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    pclose(f);

    char response[BUFFER_SIZE] = {0};
    strncat(response, HEADER, BUFFER_SIZE - strlen(response));
    strncat(response, buffer, BUFFER_SIZE - strlen(response));
    response[strlen(response) - 1] = 0;
    send(c_socket, response, strlen(response), 0);
}

void get_cpu_name(int c_socket)
{
    FILE *f = popen("cat /proc/cpuinfo | grep 'model name' | head -n 1 | awk -F ': ' '{print $2}'", "r");
    if (f == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    char buffer[BUFFER_SIZE] = {0};
    char *res = fgets(buffer, BUFFER_SIZE, f);
    if (res == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    pclose(f);

    char response[BUFFER_SIZE] = {0};
    strncat(response, HEADER, BUFFER_SIZE - strlen(response));
    strncat(response, buffer, BUFFER_SIZE - strlen(response));
    response[strlen(response) - 1] = 0;
    send(c_socket, response, strlen(response), 0);
}

// https://stackoverflow.com/a/23376195
void get_cpu_load(int c_socket)
{

    unsigned long long int f_user, f_nice, f_system, f_idle, f_iowait, f_irq, f_softirq, f_steal, f_guest, f_guest_nice = 0;
    unsigned long long int s_user, s_nice, s_system, s_idle, s_iowait, s_irq, s_softirq, s_steal, s_guest, s_guest_nice = 0;
    int check = 0;

    FILE *f = popen("cat /proc/stat | head -n 1", "r");
    if (f == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    check = fscanf(f, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu ", &f_user, &f_nice, &f_system, &f_idle, &f_iowait, &f_irq, &f_softirq, &f_steal, &f_guest, &f_guest_nice);
    if (check != 10)
    {
        fprintf(stderr, "Error: Incorrect read of /cpu/stats\n");
        exit(1);
    }

    pclose(f);

    sleep(1);

    f = popen("cat /proc/stat | head -n 1", "r");
    if (f == NULL)
    {
        perror("Error: ");
        exit(1);
    }

    check = fscanf(f, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu ", &s_user, &s_nice, &s_system, &s_idle, &s_iowait, &s_irq, &s_softirq, &s_steal, &s_guest, &s_guest_nice);
    if (check != 10)
    {
        fprintf(stderr, "Error: Incorrect read of /cpu/stats\n");
        exit(1);
    }

    pclose(f);

    unsigned long long int f_total_idle = f_idle + f_iowait;
    unsigned long long int f_total = f_idle + f_user + f_nice + f_system + f_irq + f_softirq + f_steal;

    unsigned long long int s_total_idle = s_idle + s_iowait;
    unsigned long long int s_total = s_idle + s_user + s_nice + s_system + s_irq + s_softirq + s_steal;

    double cpu_load = 100.0 * ((((double)s_total - (double)f_total) - ((double)s_total_idle - (double)f_total_idle)) / ((double)s_total - (double)f_total));

    // TODO: Deal with boundary safe concat
    char buffer[BUFFER_SIZE] = {0};
    snprintf(buffer, BUFFER_SIZE, "%.0f", cpu_load);
    strncat(buffer, "%", BUFFER_SIZE - strlen(buffer));

    char response[BUFFER_SIZE] = {0};
    strncat(response, HEADER, BUFFER_SIZE - strlen(response));
    strncat(response, buffer, BUFFER_SIZE - strlen(response));
    send(c_socket, response, strlen(response), 0);
}

void parse_http_request(char *msg, int c_socket)
{

    if (strcmp(msg, HOSTNAME_STR) == 0)
    {
        get_hostname(c_socket);
    }

    else if (strcmp(msg, CPU_NAME_STR) == 0)
    {
        get_cpu_name(c_socket);
    }

    else if (strcmp(msg, LOAD_STR) == 0)
    {
        get_cpu_load(c_socket);
    }

    else
    {
        send(c_socket, BAD_REQ, strlen(BAD_REQ), 0);
    }
}

unsigned short parse_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect parameters, to run the program use:\n\t./hinfo port_number\n");
        exit(1);
    }

    char *check = NULL;
    unsigned long result = strtoul(argv[1], &check, 10);
    if (*check != 0)
    {
        fprintf(stderr, "The port number must be an unsigned integer.\n");
        exit(1);
    }

    if (result > USHRT_MAX)
    {
        fprintf(stderr, "The port number must be in range 0-%d.\n", USHRT_MAX);
        exit(1);
    }

    return (unsigned short)result;
}

socket_info_t prepare_socket(unsigned short port_num)
{
    socket_info_t serv;
    if ((serv.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        perror("Error while creating a server socket: ");
        exit(1);
    }

    serv.opt = 1;

    if (setsockopt(serv.socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(serv.opt), sizeof(serv.opt)))
    {
        perror("Error while setting socket options: ");
        exit(1);
    }

    serv.addr.sin_family = AF_INET;
    serv.addr.sin_port = htons(port_num);
    serv.addr.sin_addr.s_addr = INADDR_ANY;

    // TODO: Check for errors
    bind(serv.socket, (struct sockaddr *)&(serv.addr), sizeof(serv.addr));

    listen(serv.socket, 3);

    return serv;
}

int main(int argc, char **argv)
{
    unsigned short port_num = parse_args(argc, argv);

    socket_info_t serv = prepare_socket(port_num);

    int c_socket;
    while (1)
    {
        if ((c_socket = accept(serv.socket, NULL, NULL)) < 0)
        {
            perror("Error in accept");
            exit(1);
        }

        char buffer[BUFFER_SIZE] = {0};
        read(c_socket, buffer, BUFFER_SIZE);

        char *line = strtok(buffer, "\r");

        parse_http_request(line, c_socket);

        close(c_socket);
    }
    close(serv.socket);
    exit(0);
}

/*
/hostname
/cpu-name
/load
*/
