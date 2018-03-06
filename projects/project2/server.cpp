#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define FAILURE 1337


int initAddress(int port, sockaddr_in *address){

  address->sin_family = AF_INET;
  //address->sin_addr.s_addr = "localhost";
  address->sin_addr.s_addr = INADDR_ANY;
  address->sin_port = htons(port);

  //printf("address inited\n port %d\n", ntohs(address->sin_port));

  return 1;
}

// returns socket file descriptor
int initSock(int port, struct sockaddr_in *address){
  // AF_INET: 		IPv4
  // SOCK_STREAM: 	TCP
  // 0:			IP
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  
  int option = 1;

  if (sock <= 0){
    perror("socket failure");
    exit(FAILURE);
  }

  // setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
  // lets you use same port & avoid "address already in use" error
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))){
    perror("sockopt failure");
    exit(FAILURE);
  }

  initAddress(port, address);
    
  // bind sock to port
  if (bind(sock, (struct sockaddr *)address, sizeof(*address)) < 0){
    perror("binder failure");
    exit(FAILURE);
  }
  
  return sock;
}


int serve(int sock, struct sockaddr_in *address){
  int listensock;
  int l = sizeof(*address);
  printf("addrlen: %d\n", l);

  // listen(): waits for client to connect
  if (listen(sock, 1) < 0){
    perror("listen failure");
    exit(FAILURE);
  }
  else
    printf("socket %d listening.\n", (listensock));

  // accept(): accepts a valid connection
  if ((listensock = accept(sock, (struct sockaddr *)&(*address), (socklen_t*)&l)) < 0){
    perror("accept failure");
    exit(FAILURE);
  }
  else
    printf("accepting connection on socket %d\n", (listensock));


  
  return listensock;
}


void cleanup(int sock){
  // stop sending and receiving on this socket
  shutdown(sock, 2);
}


int main(int argc, char** const argv){
  // will hold info about server/port
  struct sockaddr_in address;
  int msg;
  char buff[512] = {0};
  char *howdy = "Howdy, client.";

  if (! argv[1]){
    printf("missing port argument\n");
    exit(FAILURE);
  }
  
  int port = atoi(argv[1]); 
  if (!port) { // first and only arg should be port
    perror("invalid port argument");
    exit(FAILURE);
  }
  else
    printf("Starting server on port %d\n", port);
 

  // get connected socket & start serving to the network
  int sock = initSock(port, &address);
  int servesock = serve(sock, &address);

  
  // read data from accepted connection
  msg = read(servesock, buff, 512);
  printf("%s\n", buff);
  send(servesock, howdy, strlen(howdy), 0);
  printf("Replied to client\n");

  
  cleanup(sock);

  printf("Program \"%s\" finished successfully.\n", (argv[0]));
  return 0;
}
