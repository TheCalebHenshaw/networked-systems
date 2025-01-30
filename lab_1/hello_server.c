#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_ntoa()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define PORT 8080            // Server port number
#define BUFFER_SIZE 1024     // Buffer size for messages
#define BACKLOG 10           // Number of pending connections queue will hold

int main(){
    int listenfd, connfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    char buffer[BUFFER_SIZE];
    int bytes_received, bytes_sent;

    // 1. Create a socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");

    // 2. Set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;              // IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;      // Any incoming interface
    serv_addr.sin_port = htons(PORT);            // Port number

    // 3. Bind the socket
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind");
        close(listenfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket bound to port %d.\n", PORT);

    // 4. Listen for incoming connections
    if(listen(listenfd, BACKLOG) == -1){
        perror("listen");
        close(listenfd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d.\n", PORT);

    // 5. Accept and handle incoming connections
    while(1){
        // Accept a connection
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);
        if(connfd == -1){
            perror("accept");
            continue; // Continue to accept new connections
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        // Receive data from client
        bytes_received = recv(connfd, buffer, BUFFER_SIZE - 1, 0);
        if(bytes_received == -1){
            perror("recv");
            close(connfd);
            continue;
        } else if(bytes_received == 0){
            printf("Client disconnected.\n");
            close(connfd);
            continue;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received from client: %s\n", buffer);

        // Echo the message back to the client
        bytes_sent = send(connfd, buffer, bytes_received, 0);
        if(bytes_sent == -1){
            perror("send");
        } else {
            printf("Echoed back %d bytes to client.\n", bytes_sent);
        }

        // Close the connection
        close(connfd);
        printf("Connection closed with client.\n");
    }

    // Close the listening socket (optional, never reached in this example)
    close(listenfd);
    return 0;
}
