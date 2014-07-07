#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int
main(int argc, char **argv)
{
    int fd, res;
    struct addrinfo hint, *result;
    
    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    
    
    res = getaddrinfo("10.108.103.159", "2007", &hint, &result);
    if (res == -1) {
        perror("getaddr info error");
    }
    
    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd == -1) {
        perror("socket error");
    }
    
    res = connect(fd, result->ai_addr, result->ai_addrlen); 
    if (res == -1) {
        perror("connect error");
    }
}
