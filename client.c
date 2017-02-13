# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define PORT 12345

int main(int argc, char** argv){
    // create a TCP socket
    int sd=socket(AF_INET,SOCK_STREAM,0);

    // configure settings of the server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(PORT);

    // connect the socket to the server using the address struct
    if(connect(sd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
        printf("connection error: %s (Errno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    while(1){
        printf("Type the message you would like to send. Type 'exit' to exit.\n");
        char buff[100];
        memset(buff,0,100);
        scanf("%s",buff);
        int len;
        // send message to server
        if((len=send(sd,buff,strlen(buff),0))<0){
            printf("Send Error: %s (Errno:%d)\n",strerror(errno),errno);
            exit(0);
        }
        if(strcmp(buff,"exit")==0){
            close(sd);
            break;
        }
        printf("Your message have been sent.\n");
    }
    return 0;
}
