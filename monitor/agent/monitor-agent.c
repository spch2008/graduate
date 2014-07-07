#include "../core/core.h"
#include <string.h>

static float get_cpu_info();
static float get_memory_info();

static int mToInt(char *buf);
static int get_next(char **buf);

pm_info*
getpminfo_1_svc(info_type *type, struct svc_req *rqstp)
{
    static pm_info info;
    
    info.memory_ratio = get_memory_info();
    info.cpu_ration   = get_cpu_info();
    
    return &info;
}

float get_memory_info()
{
    FILE *fp;
    int cnt, total_memory, free_memory;
    char buf[100];
    
    fp = fopen("/proc/meminfo", "r");
    
    fgets(buf, sizeof(buf), fp);
    total_memory = mToInt(buf);
    
    fgets(buf, sizeof(buf), fp);
    free_memory = mToInt(buf);
    
    return free_memory * 1.0 / total_memory;
}

float get_cpu_info()
{
    FILE *fp;
    int user, nice, system, idle;
    char *left, buf[100];
    
    fp = fopen("/proc/stat", "r");
    fgets(buf, sizeof(buf), fp);
    
    left = strstr(buf, " ");
    while (*left == ' ')
        ++left;
    
    user = get_next(&left);
    nice = get_next(&left);
    system = get_next(&left);
    idle   = get_next(&left);
    
    return (user + nice + system) * 1.0 / (user + nice + system + idle);
}

int get_next(char **buf)
{
    char *left = *buf;
    char *end;
    int   ret;
    
    while (*left == ' ')
        ++left;
        
    end = strstr(left, " ");
    *end = '\0';

    ret = atoi(left);
   
    *buf = end + 1;
    return ret;
}


int mToInt(char *buf)
{
    char *left, *right;
    
    left = strstr(buf, " ");
    while (*left == ' ')
        ++left;
    
    right = strstr(left, " ");
    *right = '\0';
    
    return atoi(left);
}
