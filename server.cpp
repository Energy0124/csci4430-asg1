# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>

# define PORT 12345

int main(int argc, char** argv){
    // create a TCP socket
    int sd=socket(AF_INET,SOCK_STREAM,0);

    // reuse server port to avoid "bind: address already in use"
    int val = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))<0){
        printf("setsocketopt error: %s (Errno:%d)\n",strerror(errno),errno);
        exit(1);
    }

    // configure settings of the server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(PORT);

    // bind the address struct to the socket
    if(bind(sd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0){
        printf("bind error: %s (Errno:%d)\n",strerror(errno),errno);
        exit(1);
    }

    // listen on the socket, with 1 max connection requests queued
    if(listen(sd,1)<0){
        printf("listen error: %s (Errno:%d)\n",strerror(errno),errno);
        exit(1);
    }

    // create a new socket for the incoming connection
    int client_sd;
    struct sockaddr_in client_addr;
    int addr_len=sizeof(client_addr);

    // accept the incoming connection
    if((client_sd=accept(sd,(struct sockaddr *) &client_addr,&addr_len))<0){
        printf("accept erro: %s (Errno:%d)\n",strerror(errno),errno);
        exit(1);
    }

    while(1){
        char buff[100];
        int len;
        // receive message from the client
        if((len=recv(client_sd,buff,sizeof(buff),0))<=0){
            printf("receive error: %s (Errno:%d)\n", strerror(errno),errno);
            exit(1);
        }
        buff[len]='\0';
        printf("RECEIVED INFO: ");
        if(strlen(buff)!=0)printf("%s\n",buff);
        if(strcmp("exit",buff)==0){
            close(client_sd);
            break;
        }
    }
    close(sd);
    return 0;
}
