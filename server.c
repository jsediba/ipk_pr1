
#include "server.h" 
#define BUFFER_SIZE 1024

void get_hostname()
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

    printf("%s", buffer);

    pclose(f);
}

void get_cpu_name()
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

    printf("%s", buffer);
    
    pclose(f);
}

// https://stackoverflow.com/a/23376195
void get_cpu_load()
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
    if(check != 10){
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
    printf("%.10f\n", cpu_load);
}

int main(int argc, char **argv)
{
    get_hostname();
    get_cpu_name();
    get_cpu_load();
    return 0;
}
