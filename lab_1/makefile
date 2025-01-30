CC=clang
CFLAGS=-Wall -Werror -W

all: hello_server hello_client

hello_server: hello_server.c
	$(CC) $(CFLAGS) -o $@ $<

hello_client: hello_client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f hello_server hello_client