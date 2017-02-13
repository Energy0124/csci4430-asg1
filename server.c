# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
#include <pthread.h>
#include <signal.h>


# define PORT 12345

static const int MAX_CONNECTION = 50;
static int message_count = 0;

// The thread argument struct
struct thread_argv {
    int client_sd;
    pthread_mutex_t *mutex;
};

void sig_handler(int signum) {
    exit(0);
}

void *respond_to_client(void *_tmp_argv) {
    // Copy thread arguments to this thread
    struct thread_argv *tmp_argv = (struct thread_argv *) _tmp_argv;
    int client_sd = tmp_argv->client_sd;
    pthread_mutex_t *mutex = tmp_argv->mutex;

    while (1) {
        char buff[100];
        int len;
        // receive message from the client
        if ((len = recv(client_sd, buff, sizeof(buff), 0)) <= 0) {
            printf("receive error: %s (Errno:%d)\n", strerror(errno), errno);
            exit(1);
        }
        // Lock the mutex
        pthread_mutex_lock(mutex);
        ++message_count;
        // Unlock the mutex
        pthread_mutex_unlock(mutex);

        buff[len] = '\0';
        printf("[ %d ]", message_count);
        printf("RECEIVED INFO: ");
        if (strlen(buff) != 0)printf("%s\n", buff);

        if (strcmp("exit", buff) == 0) {
            close(client_sd);
            break;
        }
    }

    return 0;

}


int main(int argc, char **argv) {

    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGQUIT, sig_handler);

    // create a TCP socket
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    // reuse server port to avoid "bind: address already in use"
    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        printf("setsocketopt error: %s (Errno:%d)\n", strerror(errno), errno);
        exit(1);
    }

    // configure settings of the server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // bind the address struct to the socket
    if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("bind error: %s (Errno:%d)\n", strerror(errno), errno);
        exit(1);
    }

    // listen on the socket, with 1 max connection requests queued
    if (listen(sd, MAX_CONNECTION) < 0) {
        printf("listen error: %s (Errno:%d)\n", strerror(errno), errno);
        exit(1);
    }

    // create a new socket for the incoming connection
    int client_sd;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    while (1) {
        // accept the incoming connection
        if ((client_sd = accept(sd, (struct sockaddr *) &client_addr, &addr_len)) < 0) {
            printf("accept erro: %s (Errno:%d)\n", strerror(errno), errno);
            exit(1);
        }
        // Initialize a mutex for protecting the "evaluated" array from race condition
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        // Create a common thread argument struct
        struct thread_argv common_thread_argv;

        common_thread_argv.mutex = &mutex;
        common_thread_argv.client_sd = client_sd;

        // Thread ID
        pthread_t thread_id;

        // Create Threads
        pthread_create(&thread_id, NULL, (void *(*)(void *)) respond_to_client, (void *) &common_thread_argv);

    }

    close(sd);
    return 0;
}
