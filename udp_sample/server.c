# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/udp.h>

# define PORT 12333

int main(int argc, char** argv){
    // create UDP socket
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    
    // configure settings in address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    // initialize size variable which is used later
    socklen_t addrLen = sizeof(server_addr);
    
    // bind socket with address struct
    if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("bind error: %s (Errno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    char buff[100];
    char* ackBuff = "ack";

    for(int i = 0; i < 5; i++) {
        printf("Waiting heartbeat...\n");
        int len;
        
        // receive message from the client 
        if((len = recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr*)&client_addr, &addrLen)) <= 0) {
            printf("receive error: %s (Errno:%d)\n", strerror(errno),errno);
        }
        buff[len] = '\0';
        
        // print out the received message
        printf("Received heartbeat: ");
        if(strlen(buff) != 0) printf("%s\n",buff);
        
        // send response to the client
        if((len = sendto(sd, ackBuff, strlen(ackBuff), 0, (struct sockaddr*)&client_addr, addrLen)) <= 0) {
            printf("Send Error: %s (Errno:%d)\n",strerror(errno),errno);
            exit(0);
        }
        printf("The sent response: %s\n\n", ackBuff);
    }
    close(sd);
    return 0;
}
