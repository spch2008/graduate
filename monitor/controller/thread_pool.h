#include <pthread.h>

#include "../core/core.h"
/*
struct msg
{
    void *data;
};
*/

struct pool
{
    int head;
    int tail;
    int size;
    CLIENT **data;
    
    pthread_mutex_t mutex_q;
    pthread_cond_t  empty_q;
    pthread_cond_t  full_q;
    
    int free_thrs;
    int num_thrs;
    int max_thrs;
    
    pthread_mutex_t mutex_thr;
};