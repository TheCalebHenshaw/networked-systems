// NS(H) Formative Exercise 1 - hello_client.c
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 1500

int main(int argc, char *argv[]) {
  int                      i; 
  ssize_t                  wlen;
  int                      fd;
  char                    *buf = malloc(BUFLEN);
  struct addrinfo          hints, *ai, *ai0;
#ifdef __APPLE__
  int flags = 0;  // macOS doesn't support MSG_NOSIGNAL
#else
  int flags = MSG_NOSIGNAL;
#endif

  if (argc != 2) {
    printf("Usage: %s <hostname>\n", argv[0]);
    return 1;
  }

  // Look up the IP address of the hostname specified on the command line
  memset(&hints, 0, sizeof(hints));
  hints.ai_family    = PF_UNSPEC;
  hints.ai_socktype  = SOCK_STREAM;
  if ((i = getaddrinfo(argv[1], "5001", &hints, &ai0)) != 0) {
    printf("Unable to look up IP address: %s", gai_strerror(i));
    return 2;
  }

  // Try to connect to each address in turn
  for (ai = ai0; ai != NULL; ai = ai->ai_next) {
    // Create a TCP/IP socket
    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd == -1) {
      continue;       // Try the next address
    }

    // Connect to the server
    if (connect(fd, ai->ai_addr, ai->ai_addrlen) == -1) {
      close(fd);
      continue;       // Try the next address
    }

    // Send "Hello, world!"
    snprintf(buf, BUFLEN, "Hello, world!");
    wlen = send(fd, buf, strlen(buf), flags);
    if (wlen == -1) {
      perror("Unable to send request");
      close(fd);
      return 2;
    }

    // Close the connection and exit
    freeaddrinfo(ai0);
    close(fd);
    free(buf);
    return 0;
  }

  // Couldn't connect
  printf("Unable to connect\n");
  return 1;
}

// vim: set ts=2 tw=0 ai et:
