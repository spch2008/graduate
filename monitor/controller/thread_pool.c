#include "thread_pool.h"

void init(struct pool *p, int s);
int  add_task(struct pool *p, void *m);
static void create_thread(struct pool *p);
static void *build_rpc(void *arg);


void init_pool(struct pool *p, int s)
{
    // tmp
    s = 10;
    
    p->head = p->tail = 0;
    p->size = s;
    
    p->data = malloc(p->size * sizeof(void*));
    if (p->data == 0) {
        perror("malloc error");
    }
    
    p->free_thrs = 0;
    p->num_thrs  = 0;
    p->max_thrs  = s;
    
    pthread_mutex_init(&(p->mutex_q), NULL);
    pthread_cond_init(&(p->empty_q), NULL);
    pthread_cond_init(&(p->full_q), NULL);
    
    pthread_mutex_init(&(p->mutex_thr), NULL);
}

void *build_rpc(void *arg)
{
    CLIENT *cl;
    pm_info *info;
    struct pool *p;

    p = (struct pool*)arg;

    pthread_mutex_lock(&p->mutex_thr);
    while (p->head == p->tail) {
        pthread_cond_wait(&p->full_q, &p->mutex_thr);
    }

    cl = p->data[p->head];
    p->head = (p->head + 1) % p->size;
    
    pthread_mutex_unlock(&p->mutex_thr);
    
    pthread_cond_signal(&p->empty_q);
    
    info_type type = 0;
    
    if ((info = getpminfo_1(&type, cl)) == NULL) {
        perror("getpminfo error\n");
    }
    printf("cpu_info : %f\n", info->cpu_ration);
}

void create_thread(struct pool *p)
{
    pthread_t tid;
    pthread_create(&tid, NULL, build_rpc, p);
    
    pthread_join(tid, NULL);
}


int  add_task(struct pool *p, void *m)
{
    if (p == NULL || m == NULL)
        return -1;
      

    pthread_mutex_lock(&p->mutex_q);
    while (((p->tail + 1) % p->size) == p->head) {
        pthread_cond_wait(&p->empty_q, &p->mutex_q);
    }
    
    p->data[p->tail] = m;
    p->tail = (p->tail + 1) % p->size;
    
    
    pthread_mutex_unlock(&p->mutex_q);
    
    pthread_cond_signal(&p->full_q);
    
    pthread_mutex_lock(&p->mutex_thr);
    if (p->free_thrs == 0) {
        // build thread
        pthread_mutex_unlock(&p->mutex_thr);
        create_thread(p);
        
    }
    else {
        p->free_thrs--;
        pthread_mutex_unlock(&p->mutex_thr);
    }
    
    return 0;
}