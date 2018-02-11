// chatClient.cpp
// a chat client written in C++
// Brock Smedley 2018

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// connects to server at port and returns the socket
int init(int port){
	struct sockaddr_in address;
	int sock = 0;
	struct sockaddr_in serv_addr;


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	  printf("\n ERROR: Could not create socket");
	  return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// convert ip to binary
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
	  printf("\nInvalid address\n");
	  return -1;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
	  printf("\nConnection Failed \n");
	  return -1;
	}

	return sock;
}

// sends and receives messages to/from the server specified at socket sock
int chat(int sock){
  //char message[] = "Ello m8";
  char buffer[500] = {0};
  char message[510] = {0};
  int reply;
  
  // get user handle
  char handle[11];
  printf("handle: ");
  fgets(handle, 10, stdin);
  printf("Hello, %s.", handle);
  // get rid of newline
  handle[strlen(handle)-1] = 0;

  while (true){
    // add handle to message
    strcpy(message, handle);
    strcat(message, "> ");

    // get user message
    printf("message: ");
    fgets(buffer, 500, stdin);
    
    // get rid of newline
    buffer[strlen(buffer)-1] = 0;

    // check for quit command
    if (strcmp(buffer, "\\quit") == 0){
      return 1;
    }

    // add user input to message
    strcat(message, buffer);

    // send message
    send(sock, message, strlen(message), 0);

    // clear buffer
    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(message));

    // read reply
    reply = read(sock, buffer, 512);
    printf("%s\n", buffer);
  }
  return 0;
}

// NOT USED: Reads port from file
int getPort(){
  FILE *fp;
  static char buffer[10];
  int i = 0;
  char c;

  fp = fopen("port.txt", "r");
  if (fp){
    while ((c = getc(fp)) != EOF){
      buffer[i] = c;
      i++;
    }
    fclose(fp);
  }
  return atoi(buffer);
}

// driver function; initializes connection and starts chat
int main(int argc, char const *argv[])
{
  int port = atoi(argv[1]);
  printf("%d\n",port);
  int sock = init(port);
  if (sock > 0)
    chat(sock);
  return 0;
}

/*
 * Special thanks to https://www.geeksforgeeks.org/socket-programming-cc/ for the boilerplate code.
 */
