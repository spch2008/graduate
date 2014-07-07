#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define BACK_LOG 20


static int handle_agent(unsigned long);


int
main(int argc, char **argv)
{
    int fd, res, clientaddrlen;
    struct addrinfo hint, *result;
    struct sockaddr_in clientaddr;
    
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;
    
    res = getaddrinfo(0, "1989", &hint, &result);
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
    
    clientaddrlen = sizeof(struct sockaddr_in);
    while (1) {
        res = accept(fd, (struct sockaddr*)&clientaddr, &clientaddrlen);
        if (res == -1) {
            perror("accept error");
        }
        
        handle_agent(clientaddr.sin_addr.s_addr);
    }
}

int handle_agent(unsigned long ipv4_addr)
{
    char addr_str[20];
    memset(addr_str, 0, sizeof(addr_str));
    
    sprintf(addr_str, "%d.%d.%d.%d", 
            ipv4_addr & 0xFF,
            (ipv4_addr >> 8) & 0xFF,
            (ipv4_addr >> 16) & 0xFF,
            (ipv4_addr >> 24) & 0xFF);
               
    
    
    printf("%s\n", addr_str);
}
