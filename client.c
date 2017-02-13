#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

# define PORT 12345

static const int BUFF_SIZE = 130;
int sd = 0;

void sig_handler(int signum) {
    close(sd);
    exit(0);
}

int main(int argc, char **argv) {

    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGQUIT, sig_handler);

    // create a TCP socket
    sd = socket(AF_INET, SOCK_STREAM, 0);

    // configure settings of the server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    struct hostent *he = gethostbyname(argv[1]);
//    struct in_addr ** addrList;
//    addrList=(struct in_addr**)he->h_addr_list;
//    server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*addrList[0]));
    memcpy(&server_addr.sin_addr, he->h_addr_list[0], (size_t) he->h_length);
    server_addr.sin_port = htons((uint16_t) atoi(argv[2]));

    // connect the socket to the server using the address struct
    if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("connection error: %s (Errno:%d)\n", strerror(errno), errno);
        exit(0);
    }

    while (1) {
//        printf("Type the message you would like to send. Type 'exit' to exit.\n");
        char buff[BUFF_SIZE];
        memset(buff, 0, BUFF_SIZE);
        char *result = fgets(buff, BUFF_SIZE, stdin);
        int len;
        // send message to server
        if ((len = send(sd, buff, strlen(buff), 0)) < 0) {
            printf("Send Error: %s (Errno:%d)\n", strerror(errno), errno);
            exit(0);
        }
        if (feof(stdin) || result == NULL) {
            buff[0] = 4;
            buff[1] = '\0';
            if ((len = send(sd, buff, strlen(buff), 0)) < 0) {
                printf("Send Error: %s (Errno:%d)\n", strerror(errno), errno);
                exit(0);
            }
            close(sd);
            break;
        }
//        printf("Your message have been sent.\n");
    }
    return 0;
}
