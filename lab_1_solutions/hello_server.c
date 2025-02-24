// NS(H) Formative Exercise 1 - hello_server.c
// Worked Answer
// 
// Copyright (c) 2009-2024 University of Glasgow
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

#define BUFLEN 1500

int main(void) {
  int                 fd,   conn_fd; 
  ssize_t             rlen;
  struct sockaddr_in  addr, conn_addr;
  socklen_t           conn_addr_len;
  char                buf[BUFLEN];
  int                 flags = 0;

  // Create a TCP/IP socket
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("Unable to create socket");
    return 1;
  }

  // Bind to port 5001
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(5001);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("Unable to bind to port");
    return 1;
  }

  // Listen for a connection
  if (listen(fd, 1) == -1) {
    perror("Unable to listen for connection");
    return 1;
  }

  // Accept the first connection
  conn_addr_len= sizeof(struct sockaddr);
  if ((conn_fd = accept(fd, (struct sockaddr *) &conn_addr, &conn_addr_len)) == -1){
    perror("Unable to accept connection");
    return 1;
  }

  // Retrieve the data
  while ((rlen = recv(conn_fd, buf, BUFLEN, flags)) > 0) {
    int i;
    for (i = 0; i < rlen; i++) {
      printf("%c", buf[i]);
    }
    printf("\n");
  }

  // Close the connection, and the underlying listening socket
  close(conn_fd);
  close(fd);

  return 0;
}

// vim: set ts=2 tw=0 ai et:
