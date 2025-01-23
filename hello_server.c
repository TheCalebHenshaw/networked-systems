#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Include arpa/inet.h for inet_addr()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>




int main(){
    int listenfd, connfd;
    struct sockaddr_in servaddr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    
    //here create a socket
    listenfd= socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd== -1){
        perror("socket");
        exit(1);
    }

    //Set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY means any address for binding
    serv_addr.sin_port = htons(8080) //port number


    //Bind the socket
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    //Listen for incoming connections
    int backlog = 10;
    if(listen(listenfd,backlog)==-1){
        perror("listen");
        exit(1);
    }
    printf("Server listening on port 8080 \n");

    while(1){
        //Accept a connection
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);
        if(connfd == -1){
            perror("accept");
            exit(1);
            continue;
        }

    }
}




