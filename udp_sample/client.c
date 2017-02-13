# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/udp.h>
# include <arpa/inet.h>

# define IPADDR "127.0.0.1"
# define PORT 12333 

int main(int argc, char** argv){
    // create a UDP socket
    int sd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in server_addr;
   
    // configure settings in address struct
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IPADDR);
    server_addr.sin_port = htons(PORT);

    // initialize size variable which is used later
    socklen_t addrLen = sizeof(server_addr);
    
    char recvBuff[100];
    char* buff = "hello";
    
    for(int i = 0; i < 5; i++) {
        int len;
        // send message to server
        printf("Say 'hello' to server.\n");
        if((len = sendto(sd, buff, strlen(buff), 0, (struct sockaddr*)&server_addr, addrLen)) <= 0) {
            printf("Send Error: %s (Errno:%d)\n",strerror(errno),errno);
            exit(0);
        }
    
        // receiver message from server 
        if((len = recvfrom(sd, recvBuff, sizeof(recvBuff)-1, 0, NULL, NULL)) < 0) {
            printf("Recv Error: %s (Errno:%d)\n", strerror(errno), errno);
            exit(0);
        }else{
            recvBuff[len] = '\0';
            printf("Received response from server: %s\n\n", recvBuff);
        }
        sleep(3);
    }
    close(sd);
    return 0;
}
