#include "../core/core.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include "thread_pool.h"
#include "single_list.h"

#define BACK_LOG 20


static void accept_client(int fin, int fout);
static int  build_socket();
static void create_thread(char *ip);
static void build_rpc(void *arg);
static void init_signal();
static void collect_info_handler(int);

struct pool thread_pool;
struct list_node *head;

int
main(int argc, char **argv)
{
    pid_t pid;
    int input[2], output[2], res; /* input output base on father process */
    
    /* create pipe */
    pipe(input);
    pipe(output);
    
    if ((pid = fork()) == -1) {
        perror("fork error");
        
    } else if (pid == 0) {  /* child */
        close(input[0]);
        close(output[1]);
        accept_client(output[0], input[1]);
        
    }
    
    close(input[1]);
    close(output[0]);
    
    int fin  = input[0];
    int fout = output[1];
    
    /* init thread pool */
    init_pool(&thread_pool);
    
    /* init single list */
    init_list(&head);
    
    /* init singal */
    init_signal();
    
    char addr_str[20];
    unsigned long ip_addr;
    while (1) {
        res = read(fin, &ip_addr, sizeof(ip_addr));
        if (res <= 0) {
            perror("read error");
        }
      
        memset(addr_str, 0, sizeof(addr_str));
    
        sprintf(addr_str, "%d.%d.%d.%d", 
             ip_addr & 0xFF,
            (ip_addr >> 8) & 0xFF,
            (ip_addr >> 16) & 0xFF,
            (ip_addr >> 24) & 0xFF);
        //printf("%s\n", addr_str);
        build_rpc(addr_str);
    }
}

void init_signal()
{
    if (signal(SIGUSR1, collect_info_handler) == SIG_ERR) {
        perror("signal error");
    }
}


void collect_info_handler(int signo)
{
    list_node *curr = head->next;
       
    while (curr != NULL) {
        CLIENT *cl = (CLIENT*)curr->data;
        
        add_task(&thread_pool, cl);
        
        curr = curr->next;
    }
}


void build_rpc(void *arg)
{
    char *ip_addr = (char*)arg;
    
    CLIENT *cl;
    
    cl = clnt_create(ip_addr, MONITOR_PROG, MONITOR_VERS, "tcp");
    if (cl == NULL) {
        perror("clnt_create error");
        exit(0);
    }
    
    //add_task(&thread_pool, cl);
    add_list(head, cl);
}

void accept_client(int fin, int fout)
{
    char addr_str[20];
    int fd, clientaddrlen, res;
    struct sockaddr_in clientaddr;
    
    fd = build_socket();
    
    clientaddrlen = sizeof(struct sockaddr_in);
    while (1) {
        res = accept(fd, (struct sockaddr*)&clientaddr, &clientaddrlen);
        if (res == -1) {
            perror("accept error");
        }
          
        write(fout, &clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr));
    }   
}

int  build_socket()
{
    int fd, res;
    struct addrinfo hint, *result;
    
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;
    
    res = getaddrinfo(0, "2000", &hint, &result);
    if (res == -1) {
        perror("can not get address info");
    }
    
    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd == -1) {
        perror("socket error\n");
    }
    
    res = bind(fd, result->ai_addr, result->ai_addrlen);
    if (res == -1) {
        perror("bind error");
    }
    
    freeaddrinfo(result);
    
    res = listen(fd, BACK_LOG);
    if (res == -1) {
        perror("listen error");
    }
    
    return fd;
}
