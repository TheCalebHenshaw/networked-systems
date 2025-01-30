#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For exit()
#include <string.h>     // For memset(), strlen()
#include <unistd.h>     // For close()
#include <sys/types.h>  // For data types
#include <sys/socket.h> // For socket(), connect(), send(), recv()
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>  // For inet_pton()


#define SERVER_IP "127.0.0.1" //LOCALHOST
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024 //buffer size for messages



int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];

    int bytes_sent, bytes_received;

    //Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");


    //Define server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //ipv4
    serv_addr.sin_port = htons(SERVER_PORT); //port number

    //Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    //Connect to server
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    //Send a message to the server
    printf("Enter a message to send to the server: ");
    fgets(send_buffer, BUFFER_SIZE, stdin);
    send_buffer[strcspn(send_buffer, "\n")] = '\0'; //remove newline character

    bytes_sent = send(sockfd, send_buffer, strlen(send_buffer), 0);
    if(bytes_sent==-1){
        perror("send");
        exit(EXIT_FAILURE);
    }
    printf("Sent %d bytes to server\n", bytes_sent);

    // 5. Receive Data from Server
    bytes_received = recv(sockfd, recv_buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else if (bytes_received == 0) {
        printf("Server closed the connection.\n");
    } else {
        recv_buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received from server: %s\n", recv_buffer);
    }

    // 6. Close the Connection
    close(sockfd);
    printf("Connection closed\n");
    return 0;
}
